package commands

import (
	"github.com/stretchr/testify/require"
	"lab3sem3/internal/testutil"
	"os"
	"path/filepath"
	"strconv"
	"testing"
)

func TestInvalidCommandsPreserveFiles(t *testing.T) {
	cases := [][]string{
		{"FInitList", "FAddEnd", "FAddAfter -1 x", "FAddBefore 5 x", "FAddIndex x -1", "FAddIndex x 9", "FDeleteFirst", "FDeleteBefore 0", "FBad"},
		{"LInitList", "LAddEnd", "LAddBefore -1 x", "LAddAfter 5 x", "LAddIndex x -1", "LAddIndex x 9", "LDeleteFirst", "LDeleteIndex 9", "LBad"},
		{"SInitStack", "SPop", "STop", "SPush", "SBad"},
		{"Q1InitQueue", "Q1Insert", "Q1Bad"},
		{"MInitArray 2", "MGet -1", "MGet 0", "MGet 1x", "MGet \"\"", "MReplace a 0", "MAddIndex a -1", "MInitArray 0", "MAddEnd", "MBad"},
		{"TInitTree", "TAdd 2147483648", "TAdd", "TBad"},
		{"HInitTable 8", "HInsert a", "HInitTable 0", "HBad"},
		{"CInitTable", "CInsert", "CBad"},
	}
	for _, queries := range cases {
		t.Run(queries[0], func(t *testing.T) {
			file := filepath.Join(t.TempDir(), "state.json")
			require.NoError(t, ProcessCommand(file, queries[0]))
			before, err := os.ReadFile(file)
			require.NoError(t, err)
			for _, query := range queries[1:] {
				require.Error(t, ProcessCommand(file, query), query)
				after, err := os.ReadFile(file)
				require.NoError(t, err)
				require.Equal(t, before, after)
			}
		})
	}
}
func TestListBoundaryAndQuotedValues(t *testing.T) {
	for _, prefix := range []string{"F", "L"} {
		file := filepath.Join(t.TempDir(), "state.json")
		require.NoError(t, ProcessCommand(file, prefix+"InitList"))
		require.NoError(t, ProcessCommand(file, prefix+`AddIndex "Тестовая строка" 0`))
		for _, query := range []string{prefix + "DeleteBefore 0", prefix + "DeleteAfter 0"} {
			require.Error(t, ProcessCommand(file, query))
		}
		output := testutil.Capture(t, func() { require.NoError(t, ProcessCommand(file, prefix+"Print")) })
		require.Equal(t, "Тестовая строка\n", output)
	}
}
func TestCuckooCycleKeepsSavedKeys(t *testing.T) {
	file := filepath.Join(t.TempDir(), "cuckoo.json")
	require.NoError(t, ProcessCommand(file, "CInitTable"))
	rejected := false
	for i := 0; i < 100; i++ {
		before, err := os.ReadFile(file)
		require.NoError(t, err)
		if err = ProcessCommand(file, "CInsert "+strconv.Itoa(i)); err != nil {
			require.Contains(t, err.Error(), "цикл")
			after, err := os.ReadFile(file)
			require.NoError(t, err)
			require.Equal(t, before, after)
			rejected = true
			break
		}
	}
	require.True(t, rejected)
}
