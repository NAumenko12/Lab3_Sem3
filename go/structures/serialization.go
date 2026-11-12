package structures

import (
	"encoding/json"
	"os"
)

type Serializable interface {
	Save(string) error
	Load(string) error
}

func saveJSON(filename string, value any) error {
	data, err := json.MarshalIndent(value, "", "  ")
	if err != nil {
		return err
	}
	return os.WriteFile(filename, append(data, '\n'), 0644)
}

func loadJSON(filename string, value any) error {
	data, err := os.ReadFile(filename)
	if err != nil {
		return err
	}
	return json.Unmarshal(data, value)
}

type listState struct {
	Type   string   `json:"type"`
	Values []string `json:"values"`
}
