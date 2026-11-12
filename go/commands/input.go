package commands

import (
	"fmt"
	"strconv"
	"strings"
)

type arguments struct {
	values []string
	index  int
	err    error
}

func splitQuery(text string) ([]string, error) {
	var values []string
	space := func(c byte) bool { return strings.ContainsRune(" \t\r\n\v\f", rune(c)) }
	for i := 0; i < len(text); {
		if space(text[i]) {
			i++
			continue
		}
		var value strings.Builder
		if text[i] == '"' {
			i++
			closed := false
			for i < len(text) {
				c := text[i]
				i++
				if c == '"' {
					closed = true
					break
				}
				if c == '\\' {
					if i == len(text) {
						return nil, fmt.Errorf("незаконченная escape-последовательность")
					}
					c = text[i]
					i++
				}
				value.WriteByte(c)
			}
			if !closed {
				return nil, fmt.Errorf("не закрыта кавычка")
			}
			if i < len(text) && !space(text[i]) {
				return nil, fmt.Errorf("после кавычки нужен пробел")
			}
		} else {
			for i < len(text) && !space(text[i]) {
				value.WriteByte(text[i])
				i++
			}
		}
		values = append(values, value.String())
	}
	return values, nil
}
func (input *arguments) value() string {
	if input.err != nil {
		return ""
	}
	if input.index >= len(input.values) {
		input.err = fmt.Errorf("не хватает аргументов команды")
		return ""
	}
	value := input.values[input.index]
	input.index++
	return value
}
func (input *arguments) number() int {
	value := input.value()
	if input.err != nil {
		return 0
	}
	number, err := strconv.ParseInt(value, 10, 32)
	if err != nil {
		input.err = fmt.Errorf("ожидалось целое число типа int")
		return 0
	}
	return int(number)
}
func (input *arguments) end() error {
	if input.err != nil {
		return input.err
	}
	if input.index != len(input.values) {
		return fmt.Errorf("лишние аргументы команды")
	}
	return nil
}
