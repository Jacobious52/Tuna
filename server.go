package main

import (
	"bufio"
	"encoding/json"
	"errors"
	"fmt"
	"log"
	"net/http"
	"os"
	"os/signal"
	"strings"
	"syscall"

	"github.com/antage/eventsource"
)

const pipeName = "/tmp/tuna"

func createPipe() {
	e := syscall.Mkfifo(pipeName, 0666)
	if e != nil {
		log.Fatalln("Failed to create pipe", pipeName)
	}
	log.Println("Created pipe")
}

func stop() {
	log.Println("end..")
	os.Remove(pipeName)
	os.Exit(0)
}

func listenForInput(msg chan string) {
	var line string
	for {
		fmt.Print("msg: ")

		reader := bufio.NewReader(os.Stdin)
		bytes, _, _ := reader.ReadLine()
		line = string(bytes)

		if line == "exit" {
			break
		}
		msg <- line
	}
	close(msg)
}

func listenForPipe(msg chan string) {
	file, err := os.OpenFile(pipeName, os.O_RDONLY, os.ModeNamedPipe)
	if err != nil {
		log.Fatalln("could not open file to pipe.", err)
	}
	defer file.Close()
	defer close(msg)

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		log.Println("message", line)
		if len(line) > 0 {
			msg <- line
		}
	}
}

// forwardFromPipe forwards a message from the api to the client
func forwardFromPipe(es eventsource.EventSource, listen func(chan string)) {
	createPipe()

	msg := make(chan string)
	go listen(msg)

	for {
		message, ok := <-msg
		if !ok {
			log.Println("API disconnected.")
			os.Remove(pipeName)
			return
		}

		if p, e := parseMessage(message); e == nil {
			es.SendEventMessage(jsonify(p), "", "")
		} else {
			log.Fatalln("Failed to parse message", e)
		}
	}
}

type testJSON struct {
	Command string

	Result      string
	Description string
	Expected    string
	Got         string
}

type logJSON struct {
	Command string

	Level   string
	Message string
}

func jsonify(v interface{}) string {
	bytes, err := json.Marshal(v)
	if err != nil {
		log.Fatalln("Failed to marshal json.", err)
	}
	return string(bytes)
}

func parseMessage(msg string) (interface{}, error) {
	tokens := strings.Split(msg, "//")

	if len(tokens) == 0 {
		return nil, errors.New("no tokens")
	}

	switch tokens[0] {
	case "TEST":
		if len(tokens) != 5 {
			return nil, errors.New("not enough args for test command")
		}
		return testJSON{tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]}, nil
	case "LOG":
		if len(tokens) != 3 {
			return nil, errors.New("not enough args for log command")
		}
		return logJSON{tokens[0], tokens[1], tokens[2]}, nil
	default:
		return nil, errors.New("unknown command")
	}
}

func main() {
	// create sig trap
	trap := make(chan os.Signal, 2)
	signal.Notify(trap, os.Interrupt, syscall.SIGTERM)
	go func() {
		<-trap
		stop()
	}()

	log.Print("Open URL http://localhost:8080/ in your browser.")

	// event source that is used to push to client
	es := eventsource.New(nil, nil)
	defer es.Close()

	http.Handle("/", http.FileServer(http.Dir("./public")))
	http.Handle("/events", es)

	// ansyc repeat forwardFromPipe
	go func() {
		for {
			forwardFromPipe(es, listenForPipe)
		}
	}()

	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		log.Fatal(err)
	}
}
