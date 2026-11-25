package structures

import (
	"os"
	"path/filepath"
	"testing"

	"github.com/stretchr/testify/require"
)

func TestBinaryRoundTrip(t *testing.T) {
	array := NewArray(4)
	array.MAddEnd("Тест\n\x00")
	first := NewFList()
	first.FAddEnd("значение")
	second := NewLList()
	second.LAddEnd("")
	stack := NewStack()
	stack.SPush("верх")
	queue := NewQueue1()
	queue.Q1Insert("первый")
	hash := NewHashTable(8)
	hash.HInsert("ключ", "значение")
	cuckoo := NewCuckooTable()
	require.True(t, cuckoo.CInsert("a"))
	tree := NewTree()
	tree.Add(8)
	tree.Add(-3)

	pairs := [][2]Serializable{
		{array, NewArray(2)}, {first, NewFList()}, {second, NewLList()},
		{stack, NewStack()}, {queue, NewQueue1()}, {hash, NewHashTable(8)},
		{cuckoo, NewCuckooTable()}, {tree, NewTree()},
	}
	for _, pair := range pairs {
		dir := t.TempDir()
		binary := filepath.Join(dir, "state.bin")
		wantJSON := filepath.Join(dir, "want.json")
		gotJSON := filepath.Join(dir, "got.json")
		require.NoError(t, pair[0].Save(wantJSON))
		require.NoError(t, pair[0].Save(binary))
		good, err := os.ReadFile(binary)
		require.NoError(t, err)
		require.Equal(t, binaryHeader, string(good[:len(binaryHeader)]))
		require.NoError(t, pair[1].Load(binary))
		want, err := os.ReadFile(wantJSON)
		require.NoError(t, err)

		for _, bad := range [][]byte{[]byte("bad"), good[:len(good)-1], append(good, 'x')} {
			require.NoError(t, os.WriteFile(binary, bad, 0644))
			require.Error(t, pair[1].Load(binary))
			require.NoError(t, pair[1].Save(gotJSON))
			got, err := os.ReadFile(gotJSON)
			require.NoError(t, err)
			require.JSONEq(t, string(want), string(got))
		}
	}
}

func TestBinaryFileErrors(t *testing.T) {
	dir := t.TempDir()
	name := filepath.Join(dir, "data.bin")
	require.Error(t, NewStack().Load(name))
	require.Error(t, saveState(name, make(chan int)))
	require.Error(t, NewStack().Save(filepath.Join(dir, "missing", "data.bin")))

	require.NoError(t, os.Mkdir(name, 0755))
	require.Error(t, NewStack().Save(name))
	require.NoError(t, NewArray(3).Save(filepath.Join(dir, "array.bin")))
	require.Error(t, NewFList().Load(filepath.Join(dir, "array.bin")))
}

func TestLegacyJSONWithBinaryExtension(t *testing.T) {
	name := filepath.Join(t.TempDir(), "legacy.bin")
	require.NoError(t, os.WriteFile(name, []byte(`{"type":"flist","values":["старые данные"]}`), 0644))
	stack := NewStack()
	require.NoError(t, stack.Load(name))
	require.Equal(t, "старые данные", stack.STop())
	require.NoError(t, stack.Save(name))
	data, err := os.ReadFile(name)
	require.NoError(t, err)
	require.Equal(t, binaryHeader, string(data[:len(binaryHeader)]))
}
