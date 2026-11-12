package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	if err := check(os.Args[1:]); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
}
func check(args []string) error {
	if len(args) != 2 {
		return fmt.Errorf("нужны профиль покрытия и минимальный процент")
	}
	minimum, err := strconv.ParseFloat(args[1], 64)
	if err != nil {
		return err
	}
	file, err := os.Open(args[0])
	if err != nil {
		return err
	}
	defer file.Close()
	total, covered := map[string]int{}, map[string]int{}
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		fields := strings.Fields(scanner.Text())
		if len(fields) != 3 {
			continue
		}
		position := strings.LastIndex(fields[0], ":")
		if position < 0 {
			return fmt.Errorf("неверная строка профиля")
		}
		name := fields[0][:position]
		statements, err := strconv.Atoi(fields[1])
		if err != nil {
			return err
		}
		hits, err := strconv.Atoi(fields[2])
		if err != nil {
			return err
		}
		total[name] += statements
		if hits > 0 {
			covered[name] += statements
		}
	}
	if err := scanner.Err(); err != nil {
		return err
	}
	if len(total) == 0 {
		return fmt.Errorf("профиль не содержит рабочего кода")
	}
	failed := false
	for name, count := range total {
		percent := 100 * float64(covered[name]) / float64(count)
		fmt.Printf("%s: %.1f%%\n", name, percent)
		if percent < minimum {
			failed = true
		}
	}
	if failed {
		return fmt.Errorf("покрытие отдельного файла ниже %.0f%%", minimum)
	}
	return nil
}
