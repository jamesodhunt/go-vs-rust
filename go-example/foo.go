//--------------------------------------------------------------------
// Comments
//--------------------------------------------------------------------

package main

import (
	"errors"
)

type Foo struct {
	Name string
	Age  uint8
}

func NewFoo(name string, age int) (Foo, error) {
	if name == "" {
		return Foo{}, errors.New("need non blank name")
	}

	if age > 120 {
		return Foo{}, errors.New("invalid age")
	}

	return Foo{
		Name: name,
		Age:  uint8(age),
	}, nil
}
