package commands

import (
	"fmt"
	"lab3sem3/structures"
)

func runLList(filename, command string, input *arguments) error {
	list := structures.NewLList()
	if command == "LInitList" {
		if err := input.end(); err != nil {
			return err
		}
		return list.Save(filename)
	}
	if err := list.Load(filename); err != nil {
		return err
	}
	switch command {
	case "LPrint", "LPrintBac", "LPrintAddresses":
		if err := input.end(); err != nil {
			return err
		}
		if command == "LPrint" {
			list.LPrint(list.GetHead())
		} else if command == "LPrintBac" {
			list.LPrintBac(list.GetTail())
		} else {
			list.LPrintAddresses(list.GetHead())
		}
		return nil
	case "LFind":
		value := input.value()
		if err := input.end(); err != nil {
			return err
		}
		list.LFind(list.GetHead(), value)
		return nil
	case "LAddFirst", "LAddEnd", "LDeleteValue":
		value := input.value()
		if err := input.end(); err != nil {
			return err
		}
		if command == "LAddFirst" {
			list.LAddFirst(value)
		} else if command == "LAddEnd" {
			list.LAddEnd(value)
		} else {
			list.LDeleteValue(value)
		}
	case "LAddIndex":
		value, index := input.value(), input.number()
		if err := input.end(); err != nil {
			return err
		}
		if index < 0 {
			return fmt.Errorf("отрицательный индекс")
		}
		if index > 0 {
			if _, err := lNode(list.GetHead(), index-1); err != nil {
				return err
			}
		}
		list.LAddIndex(value, index)
	case "LAddBefore", "LAddAfter":
		index, value := input.number(), input.value()
		if err := input.end(); err != nil {
			return err
		}
		node, err := lNode(list.GetHead(), index)
		if err != nil {
			return err
		}
		if command == "LAddBefore" {
			list.LAddBefore(node, value)
		} else {
			list.LAddAfter(node, value)
		}
	case "LDeleteFirst", "LDeleteEnd":
		if err := input.end(); err != nil {
			return err
		}
		if _, err := lNode(list.GetHead(), 0); err != nil {
			return err
		}
		if command == "LDeleteFirst" {
			list.LDeleteFirst()
		} else {
			list.LDeleteEnd()
		}
	case "LDeleteIndex", "LDeleteBefore", "LDeleteAfter", "LDeleteNode":
		index := input.number()
		if err := input.end(); err != nil {
			return err
		}
		node, err := lNode(list.GetHead(), index)
		if err != nil {
			return err
		}
		if command == "LDeleteBefore" {
			if node.Prev == nil {
				return fmt.Errorf("перед первым узлом нет элемента")
			}
			list.LDeleteBefore(node)
		} else if command == "LDeleteAfter" {
			if node.Next == nil {
				return fmt.Errorf("после последнего узла нет элемента")
			}
			list.LDeleteAfter(node)
		} else if command == "LDeleteNode" {
			list.LDeleteNode(node)
		} else {
			list.LDeleteIndex(index)
		}
	default:
		return fmt.Errorf("неизвестная команда списка: %s", command)
	}
	return list.Save(filename)
}
