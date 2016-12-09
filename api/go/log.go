package tuna

// Info pushes a info level log message
func Info(msg string) {
	api.send(api.compose("LOG", "INFO", msg))
}

// Warn pushes a info level log message
func Warn(msg string) {
	api.send(api.compose("LOG", "WARN", msg))
}
