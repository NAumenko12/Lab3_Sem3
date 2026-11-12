package testutil

import (
	"io"
	"os"
	"testing"
)

func Capture(t testing.TB, action func()) string {
	t.Helper()
	reader, writer, err := os.Pipe()
	if err != nil {
		t.Fatal(err)
	}
	original := os.Stdout
	os.Stdout = writer
	result := make(chan string, 1)
	go func() { data, _ := io.ReadAll(reader); result <- string(data) }()
	defer func() { os.Stdout = original; writer.Close(); reader.Close() }()
	action()
	writer.Close()
	os.Stdout = original
	return <-result
}
