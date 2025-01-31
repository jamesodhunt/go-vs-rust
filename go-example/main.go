package main

import (
	"fmt"
	"os"
)

func testFoo(name, age string) error {

	foo, err := NewFoo(name, age)
	if err != nil {
		return err
	}

	fmt.Printf("Foo: %+v\n", foo)

	return nil
}

func main() {
	if len(os.Args) != 3 {
		fmt.Fprintf(os.Stderr, "Usage: %s <name> <age>\n", os.Args[0])
		os.Exit(1)
	}

	name := os.Args[1]
	age := os.Args[2]

	err := testFoo(name, age)
	if err != nil {
		fmt.Fprintf(os.Stderr, "ERROR: failed: %+v\n", err)
		os.Exit(1)
	}

	os.Exit(0)
}
