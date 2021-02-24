package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {
	args := os.Args

	if len(args) == 1{
		fmt.Println("Error")
		return
	}

	fmt.Println("Welcome to the jungle", strings.Join(args[1:], " "))
}
