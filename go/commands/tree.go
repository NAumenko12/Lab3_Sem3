package commands

import (
	"fmt"
	"lab3sem3/structures"
)

func runTree(filename, command string, input *arguments) error {
	tree := structures.NewTree()
	if command == "TInitTree" {
		if err := input.end(); err != nil {
			return err
		}
		return tree.Save(filename)
	}
	if err := tree.Load(filename); err != nil {
		return err
	}
	if command == "TAdd" || command == "TDelete" || command == "TFind" {
		value := input.number()
		if err := input.end(); err != nil {
			return err
		}
		if command == "TFind" {
			tree.Find(tree.GetRoot(), value)
			return nil
		}
		if command == "TAdd" {
			tree.Add(value)
		} else {
			tree.Delete(value)
		}
	} else {
		if err := input.end(); err != nil {
			return err
		}
		switch command {
		case "TPrint", "TPrintPryam":
			tree.PrintPryam(tree.GetRoot())
			fmt.Println()
		case "TPrintAddresses":
			tree.PrintAddresses(tree.GetRoot())
		default:
			return fmt.Errorf("неизвестная команда дерева: %s", command)
		}
		return nil
	}
	return tree.Save(filename)
}
