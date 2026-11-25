package structures

import (
	"bytes"
	"encoding/gob"
	"encoding/json"
	"fmt"
	"os"
	"path/filepath"
)

const binaryHeader = "LR3G\x01"

func saveState(filename string, value any) error {
	if filepath.Ext(filename) != ".bin" {
		return saveJSON(filename, value)
	}

	var data bytes.Buffer
	data.WriteString(binaryHeader)
	if err := gob.NewEncoder(&data).Encode(value); err != nil {
		return err
	}

	file, err := os.CreateTemp(filepath.Dir(filename), ".lab3-*.tmp")
	if err != nil {
		return err
	}
	defer os.Remove(file.Name())

	_, writeErr := file.Write(data.Bytes())
	closeErr := file.Close()
	if writeErr != nil {
		return writeErr
	}
	if closeErr != nil {
		return closeErr
	}
	return os.Rename(file.Name(), filename)
}

func loadState(filename string, value any) error {
	if filepath.Ext(filename) != ".bin" {
		return loadJSON(filename, value)
	}

	data, err := os.ReadFile(filename)
	if err != nil {
		return err
	}
	if !bytes.HasPrefix(data, []byte(binaryHeader)) {
		if bytes.HasPrefix(bytes.TrimSpace(data), []byte("{")) {
			return json.Unmarshal(data, value)
		}
		return fmt.Errorf("неверный заголовок бинарного файла")
	}

	reader := bytes.NewReader(data[len(binaryHeader):])
	if err := gob.NewDecoder(reader).Decode(value); err != nil {
		return err
	}
	if reader.Len() != 0 {
		return fmt.Errorf("лишние данные в бинарном файле")
	}
	return nil
}
