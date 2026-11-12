package commands

import (
	"fmt"
	"lab3sem3/structures"
)

func runHash(filename, command string, input *arguments) error {
	table := structures.NewHashTable(8)
	if command == "HInitTable" {
		capacity := input.number()
		if err := input.end(); err != nil {
			return err
		}
		if capacity < 1 {
			return fmt.Errorf("вместимость должна быть положительной")
		}
		table.HInitTable(capacity)
		return table.Save(filename)
	}
	if err := table.Load(filename); err != nil {
		return err
	}
	switch command {
	case "HInsert":
		key, value := input.value(), input.value()
		if err := input.end(); err != nil {
			return err
		}
		table.HInsert(key, value)
	case "HGet", "HContains", "HDelete":
		key := input.value()
		if err := input.end(); err != nil {
			return err
		}
		if command == "HDelete" {
			table.HDelete(key)
		} else {
			if command == "HContains" {
				fmt.Println(table.HContains(key))
			} else {
				value, found := table.HGet(key)
				if found {
					fmt.Println(value)
				} else {
					fmt.Println("Ключ не найден")
				}
			}
			return nil
		}
	default:
		if err := input.end(); err != nil {
			return err
		}
		switch command {
		case "HClear":
			table.HClear()
		case "HDestroy":
			table.HDestroy()
		case "HLength":
			fmt.Println(table.HLength())
			return nil
		case "HPrint":
			table.HPrint()
			return nil
		case "HPrintAddresses":
			table.HPrintAddresses()
			return nil
		default:
			return fmt.Errorf("неизвестная команда хеш-таблицы: %s", command)
		}
	}
	return table.Save(filename)
}
