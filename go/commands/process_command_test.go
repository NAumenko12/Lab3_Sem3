package commands

import (
	"github.com/stretchr/testify/require"
	"lab3sem3/internal/testutil"
	"os"
	"path/filepath"
	"testing"
)

func TestCommandsPersistAllStructures(t *testing.T) {
	cases := []struct {
		queries  []string
		expected string
	}{
		{[]string{"FInitList", "FAddEnd a", "FAddFirst b", "FAddIndex c 1", "FAddBefore 0 x", "FAddAfter 0 y", "FPrintBack", "FPrintAddresses", "FFind a", "FDeleteBefore 1", "FDeleteAfter 0", "FDeleteIndex 1", "FDeleteFirst", "FDeleteEnd", "FAddEnd z", "FDeleteValue z", "FPrint"}, ""},
		{[]string{"LInitList", "LAddEnd a", "LAddFirst b", "LAddIndex c 1", "LAddBefore 0 x", "LAddAfter 0 y", "LPrintBac", "LPrintAddresses", "LFind a", "LDeleteBefore 1", "LDeleteAfter 0", "LDeleteIndex 1", "LDeleteFirst", "LDeleteEnd", "LAddEnd z", "LDeleteValue z", "LAddEnd q", "LDeleteNode 0", "LPrint"}, ""},
		{[]string{"SInitStack", "SPush a", "SPush b", "SPrint", "SPrintAddresses", "SIsEmptyStack", "SPop", "STop"}, "a\n"},
		{[]string{"Q1InitQueue", "Q1Insert a", "Q1Insert b", "Q1PrintQueue", "Q1PrintAddresses", "Q1Front", "Q1Delete", "Q1Back"}, "b\n"},
		{[]string{"MInitArray 2", "MAddEnd a", "MAddIndex b 0", "MReplace c 1", "MPrint", "MPrintAddresses", "MResize", "MLength", "MDeleteIndex 0", "MGet 0", "MDestroy", "MAddEnd x", "MGet 0"}, "x\n"},
		{[]string{"TInitTree", "TAdd 5", "TAdd 2", "TPrint", "TPrintPryam", "TPrintAddresses", "TFind 2", "TDelete 5", "TDelete 2", "TPrint"}, "\n"},
		{[]string{"HInitTable 8", "HInsert key value", "HGet key", "HGet absent", "HLength", "HPrint", "HPrintAddresses", "HContains key", "HDelete key", "HClear", "HDestroy", "HInsert other data", "HGet other"}, "data\n"},
		{[]string{"CInitTable", "CInsert a", "CInsert l", "CInsert w", "CPrint", "CDelete l", "CContains l"}, "false\n"},
	}
	for _, test := range cases {
		t.Run(test.queries[0], func(t *testing.T) {
			file := filepath.Join(t.TempDir(), "state.json")
			output := ""
			for _, query := range test.queries {
				before, _ := os.ReadFile(file)
				require.Error(t, ProcessCommand(file, query+" extra"), query)
				after, _ := os.ReadFile(file)
				require.Equal(t, before, after, query)
				output = testutil.Capture(t, func() { require.NoError(t, ProcessCommand(file, query), query) })
			}
			require.Equal(t, test.expected, output)
		})
	}
}
func TestCommandParsingAndHelp(t *testing.T) {
	output := testutil.Capture(t, PrintHelp)
	require.Contains(t, output, "--query")
	require.Contains(t, output, "CInitTable")
	values, err := splitQuery(" SPush \"a \\\"b\\\" \\\\ c\" ")
	require.NoError(t, err)
	require.Equal(t, []string{"SPush", `a "b" \ c`}, values)
	for _, bad := range []string{"SPush \"missing", "SPush \"end\\", "SPush \"a\"b"} {
		_, err = splitQuery(bad)
		require.Error(t, err)
	}
	file := filepath.Join(t.TempDir(), "state.json")
	require.Error(t, ProcessCommand(file, " "))
	require.Error(t, ProcessCommand(file, "unknown"))
	require.Error(t, ProcessCommand(file, "\"\""))
	for _, query := range []string{"FPrint", "LPrint", "STop", "Q1Front", "MPrint", "TPrint", "HPrint", "CPrint"} {
		require.Error(t, ProcessCommand(file, query))
	}
}
