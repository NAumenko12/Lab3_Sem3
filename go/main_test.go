package main

import (
	"github.com/stretchr/testify/require"
	"lab3sem3/internal/testutil"
	"os"
	"path/filepath"
	"testing"
)

func TestRunArgumentsAndCommands(t *testing.T) {
	file := filepath.Join(t.TempDir(), "state.json")
	cases := []struct {
		args []string
		code int
	}{
		{nil, 0}, {[]string{"--help"}, 0}, {[]string{"--file"}, 1}, {[]string{"--unknown"}, 1},
		{[]string{"--file", file}, 1}, {[]string{"--query", "STop"}, 1},
		{[]string{"--file", file, "--file", file}, 1},
		{[]string{"--file", file, "--query", "SInitStack"}, 0},
		{[]string{"--file", file, "--query", "SPush 10"}, 0},
		{[]string{"--file", file, "--query", "STop"}, 0},
		{[]string{"--file", file, "--query", "SBad"}, 1},
	}
	for _, test := range cases {
		testutil.Capture(t, func() { require.Equal(t, test.code, run(test.args)) })
	}
}
func TestMainHelp(t *testing.T) {
	original := os.Args
	defer func() { os.Args = original }()
	os.Args = []string{"lab3go", "--help"}
	require.Contains(t, testutil.Capture(t, main), "--query")
}
