package structures

import (
	"github.com/stretchr/testify/require"
	"lab3sem3/internal/testutil"
	"math/rand"
	"path/filepath"
	"testing"
)

func TestTreeOperationsCopyAndJSON(t *testing.T) {
	tree := NewTree()
	for _, value := range []int{8, 3, 12, 1, 6, 10, 14, 4, 7} {
		tree.Add(value)
	}
	tree.Add(8)
	copy, assigned := tree.Copy(), NewTree()
	assigned.Add(100)
	assigned.Assign(tree)
	tree.Delete(8)
	require.Contains(t, testutil.Capture(t, func() { copy.Find(copy.GetRoot(), 8) }), "8 найден")
	require.Equal(t, "8 не найден\n", testutil.Capture(t, func() { tree.Find(tree.GetRoot(), 8) }))
	require.Contains(t, testutil.Capture(t, func() { copy.PrintPryam(copy.GetRoot()); copy.PrintAddresses(copy.GetRoot()) }), "Значение:")
	require.NotSame(t, copy.GetRoot(), assigned.GetRoot())
	assigned.Assign(assigned)
	file := filepath.Join(t.TempDir(), "tree.json")
	require.NoError(t, copy.Save(file))
	require.NoError(t, tree.Load(file))
	require.Equal(t, treeToRecord(copy.root), treeToRecord(tree.root))
	copy.InitTree()
	require.Nil(t, copy.Copy().GetRoot())
	require.NoError(t, copy.Save(file))
	require.NoError(t, tree.Load(file))
	require.Nil(t, tree.root)
}
func TestTreeMixedOperationsKeepInvariants(t *testing.T) {
	tree := NewTree()
	random := rand.New(rand.NewSource(16))
	expected := map[int]bool{}
	var visit func(*Node, map[int]bool)
	visit = func(node *Node, values map[int]bool) {
		if node != nil {
			values[node.data] = true
			visit(node.left, values)
			visit(node.right, values)
		}
	}
	for i := 0; i < 2000; i++ {
		value := random.Intn(100)
		if random.Intn(2) == 0 {
			tree.Add(value)
			expected[value] = true
		} else {
			tree.Delete(value)
			delete(expected, value)
		}
		require.False(t, isRed(tree.root))
		require.GreaterOrEqual(t, checkTree(tree.root, nil, nil), 0)
		actual := map[int]bool{}
		visit(tree.root, actual)
		require.Equal(t, expected, actual)
	}
}
