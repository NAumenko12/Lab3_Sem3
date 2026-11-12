package commands

import (
	"fmt"
	"lab3sem3/structures"
)

func runFList(filename, command string, input *arguments) error {
	list := structures.NewFList()
	if command == "FInitList" {
		if err := input.end(); err != nil {
			return err
		}
		return list.Save(filename)
	}
	if err := list.Load(filename); err != nil {
		return err
	}
	switch command {
	case "FPrint", "FPrintBack", "FPrintAddresses":
		if err := input.end(); err != nil {
			return err
		}
		if command == "FPrint" {
			list.FPrint(list.GetHead())
		} else if command == "FPrintBack" {
			list.FPrintBack(list.GetHead())
		} else {
			list.FPrintAddresses(list.GetHead())
		}
		return nil
	case "FFind":
		value := input.value()
		if err := input.end(); err != nil {
			return err
		}
		list.FFind(list.GetHead(), value)
		return nil
	case "FAddFirst", "FAddEnd", "FDeleteValue":
		value := input.value()
		if err := input.end(); err != nil {
			return err
		}
		if command == "FAddFirst" {
			list.FAddFirst(value)
		} else if command == "FAddEnd" {
			list.FAddEnd(value)
		} else {
			list.FDeleteValue(value)
		}
	case "FAddIndex":
		value, index := input.value(), input.number()
		if err := input.end(); err != nil {
			return err
		}
		if index < 0 {
			return fmt.Errorf("отрицательный индекс")
		}
		if index > 0 {
			if _, err := fNode(list.GetHead(), index-1); err != nil {
				return err
			}
		}
		list.FAddIndex(value, index)
	case "FAddBefore", "FAddAfter":
		index, value := input.number(), input.value()
		if err := input.end(); err != nil {
			return err
		}
		node, err := fNode(list.GetHead(), index)
		if err != nil {
			return err
		}
		if command == "FAddBefore" {
			list.FAddBefore(node, value)
		} else {
			list.FAddAfter(node, value)
		}
	case "FDeleteFirst", "FDeleteEnd":
		if err := input.end(); err != nil {
			return err
		}
		if _, err := fNode(list.GetHead(), 0); err != nil {
			return err
		}
		if command == "FDeleteFirst" {
			list.FDeleteFirst()
		} else {
			list.FDeleteEnd()
		}
	case "FDeleteIndex", "FDeleteBefore", "FDeleteAfter":
		index := input.number()
		if err := input.end(); err != nil {
			return err
		}
		node, err := fNode(list.GetHead(), index)
		if err != nil {
			return err
		}
		if command == "FDeleteBefore" {
			if index == 0 {
				return fmt.Errorf("перед первым узлом нет элемента")
			}
			list.FDeleteBefore(node)
		} else if command == "FDeleteAfter" {
			if node.Next == nil {
				return fmt.Errorf("после последнего узла нет элемента")
			}
			list.FDeleteAfter(node)
		} else {
			list.FDeleteIndex(index)
		}
	default:
		return fmt.Errorf("неизвестная команда списка: %s", command)
	}
	return list.Save(filename)
}
