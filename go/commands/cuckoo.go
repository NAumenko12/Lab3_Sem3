package commands

import (
	"fmt"
	"lab3sem3/structures"
)

func runCuckoo(filename, command string, input *arguments) error {
	table := structures.NewCuckooTable()
	if command == "CInitTable" {
		if err := input.end(); err != nil {
			return err
		}
		return table.Save(filename)
	}
	if err := table.Load(filename); err != nil {
		return err
	}
	if command == "CPrint" {
		if err := input.end(); err != nil {
			return err
		}
		table.CPrint()
		return nil
	}
	if command != "CInsert" && command != "CContains" && command != "CDelete" {
		return fmt.Errorf("неизвестная команда кукушки: %s", command)
	}
	key := input.value()
	if err := input.end(); err != nil {
		return err
	}
	if command == "CContains" {
		fmt.Println(table.CContains(key))
		return nil
	}
	if command == "CInsert" {
		if !table.CInsert(key) {
			return fmt.Errorf("не удалось вставить ключ: цикл вытеснений или нет места")
		}
	} else {
		table.CDelete(key)
	}
	return table.Save(filename)
}
