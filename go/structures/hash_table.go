package structures

import (
	"fmt"
	"math"
)

type HashTable struct {
	data     []*HNode
	size     int
	mesto    int
	function HashFunction
}

func NewHashTable(capacity int) *HashTable { return NewHashTableWithHash(capacity, nil) }
func NewHashTableWithHash(capacity int, function HashFunction) *HashTable {
	if function == nil {
		function = PolynomialHash{}
	}
	table := &HashTable{function: function}
	table.HInitTable(capacity)
	return table
}
func (table *HashTable) HInitTable(capacity int) {
	if capacity < 1 {
		capacity = 1
	}
	if table.function == nil {
		table.function = PolynomialHash{}
	}
	table.data = make([]*HNode, capacity)
	table.mesto = capacity
	table.size = 0
}
func (table *HashTable) HClear() {
	for i := range table.data {
		table.data[i] = nil
	}
	table.size = 0
}
func (table *HashTable) HDestroy()    { table.data = nil; table.size = 0; table.mesto = 0 }
func (table *HashTable) HLength() int { return table.size }
func (table *HashTable) resize() {
	capacity := table.mesto * 2
	data := make([]*HNode, capacity)
	for _, node := range table.data {
		for node != nil {
			next := node.Next
			index := table.function.HHash(node.Key, capacity)
			node.Next = data[index]
			data[index] = node
			node = next
		}
	}
	table.data = data
	table.mesto = capacity
}
func (table *HashTable) HInsert(key, value string) {
	if table.data == nil {
		table.HInitTable(8)
	}
	index := table.function.HHash(key, table.mesto)
	for node := table.data[index]; node != nil; node = node.Next {
		if node.Key == key {
			node.Value = value
			return
		}
	}
	table.data[index] = &HNode{key, value, table.data[index]}
	table.size++
	if table.size >= table.mesto-table.mesto/4 {
		table.resize()
	}
}
func (table *HashTable) HGet(key string) (string, bool) {
	if table.data == nil {
		return "", false
	}
	for node := table.data[table.function.HHash(key, table.mesto)]; node != nil; node = node.Next {
		if node.Key == key {
			return node.Value, true
		}
	}
	return "", false
}
func (table *HashTable) HContains(key string) bool { _, found := table.HGet(key); return found }
func (table *HashTable) HDelete(key string) {
	if table.data == nil {
		return
	}
	index := table.function.HHash(key, table.mesto)
	node := table.data[index]
	var previous *HNode
	for node != nil && node.Key != key {
		previous = node
		node = node.Next
	}
	if node == nil {
		return
	}
	if previous == nil {
		table.data[index] = node.Next
	} else {
		previous.Next = node.Next
	}
	table.size--
}
func (table *HashTable) HPrint() {
	for _, node := range table.data {
		for node != nil {
			fmt.Printf("%s -> %s\n", node.Key, node.Value)
			node = node.Next
		}
	}
}
func (table *HashTable) HPrintAddresses() {
	for i, node := range table.data {
		fmt.Printf("[%d]", i)
		for node != nil {
			fmt.Printf(" -> %s:%s (%p)", node.Key, node.Value, node)
			node = node.Next
		}
		fmt.Println()
	}
	fmt.Printf("size = %d mesto = %d\n", table.size, table.mesto)
}
func (table *HashTable) Copy() *HashTable {
	function := table.function
	if function != nil {
		function = function.Clone()
	}
	result := NewHashTableWithHash(table.mesto, function)
	if table.data == nil {
		result.HDestroy()
		return result
	}
	for i, node := range table.data {
		var previous *HNode
		for node != nil {
			current := &HNode{Key: node.Key, Value: node.Value}
			if previous == nil {
				result.data[i] = current
			} else {
				previous.Next = current
			}
			previous = current
			node = node.Next
		}
	}
	result.size = table.size
	return result
}
func (table *HashTable) Assign(other *HashTable) { *table = *other.Copy() }

type hashEntry struct {
	Key   string `json:"key"`
	Value string `json:"value"`
}
type hashState struct {
	Type     string      `json:"type"`
	Capacity int         `json:"capacity"`
	Entries  []hashEntry `json:"entries"`
}

func (table *HashTable) Save(filename string) error {
	state := hashState{Type: "hash", Capacity: table.mesto, Entries: make([]hashEntry, 0)}
	for _, node := range table.data {
		for node != nil {
			state.Entries = append(state.Entries, hashEntry{node.Key, node.Value})
			node = node.Next
		}
	}
	return saveJSON(filename, state)
}
func (table *HashTable) Load(filename string) error {
	var state hashState
	if err := loadJSON(filename, &state); err != nil {
		return err
	}
	if state.Type != "hash" || state.Capacity < len(state.Entries) || state.Capacity < 0 || state.Capacity > math.MaxInt32 {
		return fmt.Errorf("неверный размер хеш-таблицы")
	}
	function := table.function
	if function != nil {
		function = function.Clone()
	}
	loaded := NewHashTableWithHash(state.Capacity, function)
	if state.Capacity == 0 {
		loaded.HDestroy()
	}
	for _, entry := range state.Entries {
		loaded.HInsert(entry.Key, entry.Value)
	}
	if loaded.size != len(state.Entries) {
		return fmt.Errorf("повторяющиеся ключи")
	}
	*table = *loaded
	return nil
}
