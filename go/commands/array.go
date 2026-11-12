package commands

import (
	"fmt"
	"lab3sem3/structures"
)

func runArray(filename, command string, input *arguments) error {
	array := structures.NewArray(2)
	if command == "MInitArray" {
		capacity := input.number()
		if err := input.end(); err != nil {
			return err
		}
		if capacity < 1 {
			return fmt.Errorf("вместимость должна быть положительной")
		}
		array.MInitArray(capacity)
		return array.Save(filename)
	}
	if err := array.Load(filename); err != nil {
		return err
	}
	switch command {
	case "MAddEnd":
		value := input.value()
		if err := input.end(); err != nil {
			return err
		}
		array.MAddEnd(value)
	case "MAddIndex", "MReplace":
		value, index := input.value(), input.number()
		if err := input.end(); err != nil {
			return err
		}
		limit := array.MLength()
		if command == "MReplace" {
			limit--
		}
		if index < 0 || index > limit {
			return fmt.Errorf("индекс за границами массива")
		}
		if command == "MAddIndex" {
			array.MAddIndex(value, index)
		} else {
			array.MReplace(value, index)
		}
	case "MDeleteIndex", "MGet":
		index := input.number()
		if err := input.end(); err != nil {
			return err
		}
		if index < 0 || index >= array.MLength() {
			return fmt.Errorf("индекс за границами массива")
		}
		if command == "MGet" {
			fmt.Println(array.MGet(index))
			return nil
		}
		array.MDeleteIndex(index)
	default:
		if err := input.end(); err != nil {
			return err
		}
		switch command {
		case "MDestroy":
			array.MDestroy()
		case "MResize":
			array.MResize()
		case "MLength":
			fmt.Println(array.MLength())
			return nil
		case "MPrint":
			array.MPrint()
			return nil
		case "MPrintAddresses":
			array.MPrintAddresses()
			return nil
		default:
			return fmt.Errorf("неизвестная команда массива: %s", command)
		}
	}
	return array.Save(filename)
}
