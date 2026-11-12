package structures

import (
	"encoding/json"
	"github.com/stretchr/testify/require"
	"testing"
)

func TestInvalidTreeJSONKeepsOriginal(t *testing.T) {
	tree := NewTree()
	tree.Add(7)
	bad := []*treeRecord{
		{Value: 10, Red: true},
		{Value: 10, Left: &treeRecord{Value: 20}, Right: &treeRecord{Value: 15}},
		{Value: 10, Left: &treeRecord{Value: 5}, Right: &treeRecord{Value: 3}},
		{Value: 10, Left: &treeRecord{Value: 5, Red: true, Left: &treeRecord{Value: 3, Red: true}}},
		{Value: 10, Left: &treeRecord{Value: 5, Red: true, Right: &treeRecord{Value: 7, Red: true}}},
		{Value: 10, Left: &treeRecord{Value: 5}},
	}
	for _, root := range bad {
		data, err := json.Marshal(treeState{"tree", root})
		require.NoError(t, err)
		rejectJSON(t, tree, data)
	}
}
