package structures

import (
	"bytes"
	"encoding/json"
	"github.com/stretchr/testify/require"
	"os"
	"path/filepath"
	"testing"
)

func rejectJSON(t *testing.T, object Serializable, data []byte) {
	t.Helper()
	file := filepath.Join(t.TempDir(), "state.json")
	require.NoError(t, object.Save(file))
	good, err := os.ReadFile(file)
	require.NoError(t, err)
	require.NoError(t, os.WriteFile(file, data, 0644))
	require.Error(t, object.Load(file))
	require.NoError(t, object.Save(file))
	after, err := os.ReadFile(file)
	require.NoError(t, err)
	require.JSONEq(t, string(good), string(after))
}
func TestJSONRoundTripAndFailedLoads(t *testing.T) {
	array := NewArray(4)
	array.MAddEnd("Тест\nСтрока\x00")
	array.MAddEnd("")
	first := NewFList()
	first.FAddEnd("a")
	first.FAddEnd("")
	second := NewLList()
	second.LAddEnd("b")
	hash := NewHashTable(8)
	hash.HInsert("a", "value")
	tree := NewTree()
	tree.Add(8)
	tree.Add(3)
	cuckoo := NewCuckooTable()
	require.True(t, cuckoo.CInsert("a"))
	require.True(t, cuckoo.CInsert("l"))
	stack := NewStack()
	stack.SPush("x")
	queue := NewQueue1()
	queue.Q1Insert("y")
	pairs := [][2]Serializable{{array, NewArray(2)}, {first, NewFList()}, {second, NewLList()}, {hash, NewHashTable(8)}, {tree, NewTree()}, {cuckoo, NewCuckooTable()}, {stack, NewStack()}, {queue, NewQueue1()}}
	for _, pair := range pairs {
		file := filepath.Join(t.TempDir(), "state.json")
		require.NoError(t, pair[0].Save(file))
		data, err := os.ReadFile(file)
		require.NoError(t, err)
		require.NoError(t, pair[1].Load(file))
		require.NoError(t, pair[1].Save(file))
		after, err := os.ReadFile(file)
		require.NoError(t, err)
		require.JSONEq(t, string(data), string(after))
		for _, bad := range [][]byte{[]byte("broken"), []byte(`{"type":"wrong"}`), []byte("null")} {
			rejectJSON(t, pair[0], bad)
		}
		require.Error(t, pair[0].Load(file+"/missing"))
		require.Error(t, pair[0].Save(file+"/missing"))
		data = bytes.TrimSpace(data)
		for _, length := range []int{0, 1, len(data) / 2, len(data) - 1} {
			rejectJSON(t, pair[0], data[:length])
		}
	}
	require.Equal(t, "Тест\nСтрока\x00", array.MGet(0))
	require.Error(t, saveJSON(filepath.Join(t.TempDir(), "bad.json"), make(chan int)))
}
func TestInvalidArrayAndHashJSON(t *testing.T) {
	a := NewArray(2)
	a.MAddEnd("saved")
	for _, bad := range []string{`{"type":"array","capacity":-1}`, `{"type":"array","capacity":0,"values":["x"]}`, `{"type":"array","capacity":2147483648}`} {
		rejectJSON(t, a, []byte(bad))
	}
	h := NewHashTable(8)
	h.HInsert("saved", "yes")
	for _, bad := range []string{`{"type":"hash","capacity":-1}`, `{"type":"hash","capacity":2147483648}`, `{"type":"hash","capacity":0,"entries":[{"key":"a"}]}`, `{"type":"hash","capacity":8,"entries":[{"key":"a"},{"key":"a"}]}`} {
		rejectJSON(t, h, []byte(bad))
	}
	file := filepath.Join(t.TempDir(), "hash.json")
	h.HDestroy()
	require.NoError(t, h.Save(file))
	var zero HashTable
	require.NoError(t, zero.Load(file))
	require.Zero(t, zero.size)
}
func TestInvalidCuckooJSON(t *testing.T) {
	h := NewCuckooTable()
	require.True(t, h.CInsert("saved"))
	rejectJSON(t, h, []byte(`{"type":"cuckoo","first":[],"second":[]}`))
	state := cuckooState{Type: "cuckoo", First: make([]cellState, 11), Second: make([]cellState, 11)}
	state.First[(h.hash1("a")+1)%11] = cellState{"a", true}
	data, err := json.Marshal(state)
	require.NoError(t, err)
	rejectJSON(t, h, data)
	state.First = make([]cellState, 11)
	state.First[h.hash1("a")] = cellState{"a", true}
	state.Second[h.hash2("a")] = cellState{"a", true}
	data, err = json.Marshal(state)
	require.NoError(t, err)
	rejectJSON(t, h, data)
}
