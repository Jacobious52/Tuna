package tuna

import (
	"fmt"
	"reflect"
)

// Inequality interface for comparing
type Inequality interface {
	LessThan(Inequality) bool
}

// Assertion is a result of a test
type Assertion struct {
	Result   bool
	Expected string
	Got      string
}

// Equal are these two elements?
func Equal(result interface{}, expected interface{}) Assertion {
	return Assertion{reflect.DeepEqual(result, expected), fmt.Sprint(expected), fmt.Sprint(result)}
}

// Smaller are these two elements?
func Smaller(result Inequality, expected Inequality) Assertion {
	return Assertion{result.LessThan(expected), fmt.Sprint(expected), fmt.Sprint(result)}
}

// Greater are these two elements?
func Greater(result Inequality, expected Inequality) Assertion {
	return Assertion{!result.LessThan(expected), fmt.Sprint(expected), fmt.Sprint(result)}
}

// Test - create a test function
func Test(description string, a Assertion) {
	if !NODEBUG {
		pass := "FAIL"
		if a.Result {
			pass = "PASS"
		}
		api.send(api.compose("TEST", pass, description, a.Expected, a.Got))
	}
}
