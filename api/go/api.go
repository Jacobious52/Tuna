package tuna

import (
	"fmt"
	"log"
	"os"
	"strings"
)

const pipeName = "/tmp/tuna"

// NODEBUG defines whether or not to output or connect
var NODEBUG = false

// API holds writer for pipe
type API struct {
	file *os.File
}

// private static api
var api API

// Init file and prepare to write
func Init() {
	if !NODEBUG {
		var err error
		if api.file, err = os.OpenFile(pipeName, os.O_WRONLY, os.ModeNamedPipe); err != nil {
			log.Fatalln("Failed to open pipe.", err)
		}
		Info("Connected to server")
	}
}

// Close connection to named pipe
func Close() {
	if !NODEBUG {
		Info("Disconnecting form server")
		api.file.Close()
	}
}

// Send a message to the server
func (a *API) send(msg string) {
	if !NODEBUG {
		a.file.WriteString(msg)
	}
}

// Compose a api message
func (a *API) compose(args ...interface{}) string {
	format := fmt.Sprintf("%s%s\n", "%v", strings.Repeat("//%v", len(args)-1))
	return fmt.Sprintf(format, args...)
}
