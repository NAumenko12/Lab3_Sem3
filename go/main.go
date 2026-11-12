package main

import (
	"fmt"
	"lab3sem3/commands"
	"os"
)

func run(args []string) int {
	if len(args) == 0 || len(args) == 1 && args[0] == "--help" {
		commands.PrintHelp()
		return 0
	}
	filename, query := "", ""
	for i := 0; i < len(args); i++ {
		arg := args[i]
		if arg == "--file" && i+1 < len(args) && filename == "" {
			i++
			filename = args[i]
		} else if arg == "--query" && i+1 < len(args) && query == "" {
			i++
			query = args[i]
		} else {
			fmt.Fprintln(os.Stderr, "Ошибка: неизвестный, повторный или неполный параметр", arg)
			return 1
		}
	}
	if filename == "" || query == "" {
		commands.PrintHelp()
		return 1
	}
	if err := commands.ProcessCommand(filename, query); err != nil {
		fmt.Fprintf(os.Stderr, "Ошибка выполнения команды для файла %s: %v\n", filename, err)
		return 1
	}
	return 0
}

func main() {
	if code := run(os.Args[1:]); code != 0 {
		os.Exit(code)
	}
}
