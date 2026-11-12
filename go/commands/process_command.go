package commands

import (
	"fmt"
	"lab3sem3/structures"
	"strings"
)

func ProcessCommand(filename, query string) error {
	values, err := splitQuery(query)
	if err != nil {
		return err
	}
	if len(values) == 0 {
		return fmt.Errorf("пустая команда")
	}
	command := values[0]
	input := &arguments{values: values[1:]}
	switch {
	case strings.HasPrefix(command, "F"):
		return runFList(filename, command, input)
	case strings.HasPrefix(command, "L"):
		return runLList(filename, command, input)
	case strings.HasPrefix(command, "S"):
		return runStack(filename, command, input)
	case strings.HasPrefix(command, "Q1"):
		return runQueue(filename, command, input)
	case strings.HasPrefix(command, "M"):
		return runArray(filename, command, input)
	case strings.HasPrefix(command, "T"):
		return runTree(filename, command, input)
	case strings.HasPrefix(command, "H"):
		return runHash(filename, command, input)
	case strings.HasPrefix(command, "C"):
		return runCuckoo(filename, command, input)
	}
	return fmt.Errorf("неизвестная команда: %s", command)
}
func fNode(node *structures.ListNode, index int) (*structures.ListNode, error) {
	if index < 0 {
		return nil, fmt.Errorf("отрицательный индекс")
	}
	for i := 0; i < index && node != nil; i++ {
		node = node.Next
	}
	if node == nil {
		return nil, fmt.Errorf("нет узла с таким индексом")
	}
	return node, nil
}
func lNode(node *structures.LNode, index int) (*structures.LNode, error) {
	if index < 0 {
		return nil, fmt.Errorf("отрицательный индекс")
	}
	for i := 0; i < index && node != nil; i++ {
		node = node.Next
	}
	if node == nil {
		return nil, fmt.Errorf("нет узла с таким индексом")
	}
	return node, nil
}
