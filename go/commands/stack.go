package commands

import (
	"fmt"
	"lab3sem3/structures"
)

func runStack(filename, command string, input *arguments) error {
	stack := structures.NewStack()
	if command == "SInitStack" {
		if err := input.end(); err != nil {
			return err
		}
		return stack.Save(filename)
	}
	if err := stack.Load(filename); err != nil {
		return err
	}
	if command == "SPush" {
		value := input.value()
		if err := input.end(); err != nil {
			return err
		}
		stack.SPush(value)
	} else if command == "SPop" {
		if err := input.end(); err != nil {
			return err
		}
		if stack.SIsEmptyStack() {
			return fmt.Errorf("стек пуст")
		}
		stack.SPop()
	} else {
		if err := input.end(); err != nil {
			return err
		}
		switch command {
		case "STop":
			if stack.SIsEmptyStack() {
				return fmt.Errorf("стек пуст")
			}
			fmt.Println(stack.STop())
		case "SIsEmptyStack":
			fmt.Println(stack.SIsEmptyStack())
		case "SPrint":
			stack.SPrint(stack.GetHead())
		case "SPrintAddresses":
			stack.SPrintAddresses()
		default:
			return fmt.Errorf("неизвестная команда стека: %s", command)
		}
		return nil
	}
	return stack.Save(filename)
}
