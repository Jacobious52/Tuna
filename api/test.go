package main

import "./go"

// TInt int
type TInt int

// LessThan ddd
func (i TInt) LessThan(j tuna.Inequality) bool {
	return int(i) < int(j.(TInt))
}

func main() {

	tuna.Init()
	defer tuna.Close()

	tuna.Info("hello world")
	tuna.Warn("goodbye cruel world")

	tuna.Test("filtering air", tuna.Equal(42, 42))
	tuna.Test("filtering pair", tuna.Equal(42, 4442))
	tuna.Test("twins", tuna.Equal("dfsdgfsd", "dfsdgfsd"))
	tuna.Test("less thna?", tuna.Smaller(TInt(4), TInt(10)))
}
