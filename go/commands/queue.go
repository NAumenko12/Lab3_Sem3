package commands

import (
	"fmt"
	"lab3sem3/structures"
)

func runQueue(filename, command string, input *arguments) error {
	queue := structures.NewQueue1()
	if command == "Q1InitQueue" {
		if err := input.end(); err != nil {
			return err
		}
		return queue.Save(filename)
	}
	if err := queue.Load(filename); err != nil {
		return err
	}
	if command == "Q1Insert" {
		value := input.value()
		if err := input.end(); err != nil {
			return err
		}
		queue.Q1Insert(value)
	} else if command == "Q1Delete" {
		if err := input.end(); err != nil {
			return err
		}
		queue.Q1Delete()
	} else {
		if err := input.end(); err != nil {
			return err
		}
		switch command {
		case "Q1Front":
			fmt.Println(queue.Q1Front())
		case "Q1Back":
			fmt.Println(queue.Q1Back())
		case "Q1PrintQueue":
			queue.Q1PrintQueue()
		case "Q1PrintAddresses":
			queue.Q1PrintAddresses()
		default:
			return fmt.Errorf("неизвестная команда очереди: %s", command)
		}
		return nil
	}
	return queue.Save(filename)
}
