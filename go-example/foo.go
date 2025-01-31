//--------------------------------------------------------------------
// Comments
//--------------------------------------------------------------------

package main

import (
	"errors"
	"math"
	"strconv"
)

type Foo struct {
	Name string
	Age  uint8
}

const AGE_MAX = 120

func NewFoo(name, ageStr string) (Foo, error) {
	if name == "" {
		return Foo{}, errors.New("need non blank name")
	}

	if ageStr == "" {
		return Foo{}, errors.New("need non blank age")
	}

	age, err := strconv.Atoi(ageStr)
	if err != nil {
		return Foo{}, err
	}

	if age <= 0 || age > math.MaxUint8 {
		return Foo{}, errors.New("invalid age")
	}

	if age > AGE_MAX {
		return Foo{}, errors.New("nobody's that old!")
	}

	return Foo{
		Name: name,
		Age:  uint8(age),
	}, nil
}
