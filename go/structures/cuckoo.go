package structures

import "fmt"

type CuckooCell struct {
	key  string
	used bool
}
type CuckooTable struct {
	first      [11]CuckooCell
	second     [11]CuckooCell
	firstHash  HashFunction
	secondHash HashFunction
}

func NewCuckooTable() *CuckooTable { return NewCuckooTableWithHashes(nil, nil) }
func NewCuckooTableWithHashes(first, second HashFunction) *CuckooTable {
	if first == nil {
		first = CuckooHash1{}
	}
	if second == nil {
		second = CuckooHash2{}
	}
	return &CuckooTable{firstHash: first, secondHash: second}
}
func (table *CuckooTable) hash1(key string) int {
	if table.firstHash == nil {
		return (CuckooHash1{}).HHash(key, len(table.first))
	}
	return table.firstHash.HHash(key, len(table.first))
}
func (table *CuckooTable) hash2(key string) int {
	if table.secondHash == nil {
		return (CuckooHash2{}).HHash(key, len(table.second))
	}
	return table.secondHash.HHash(key, len(table.second))
}
func (table *CuckooTable) CContains(key string) bool {
	first, second := table.hash1(key), table.hash2(key)
	return table.first[first].used && table.first[first].key == key || table.second[second].used && table.second[second].key == key
}
func (table *CuckooTable) CInsert(key string) bool {
	if table.CContains(key) {
		return true
	}
	backup := *table
	current := key
	for i := 0; i < 20; i++ {
		first := table.hash1(current)
		if !table.first[first].used {
			table.first[first] = CuckooCell{current, true}
			return true
		}
		table.first[first].key, current = current, table.first[first].key
		second := table.hash2(current)
		if !table.second[second].used {
			table.second[second] = CuckooCell{current, true}
			return true
		}
		table.second[second].key, current = current, table.second[second].key
	}
	*table = backup
	return false
}
func (table *CuckooTable) CDelete(key string) {
	first, second := table.hash1(key), table.hash2(key)
	if table.first[first].used && table.first[first].key == key {
		table.first[first].used = false
	}
	if table.second[second].used && table.second[second].key == key {
		table.second[second].used = false
	}
}
func (table *CuckooTable) CPrint() {
	fmt.Println("Индекс | первая таблица | вторая таблица")
	for i := range table.first {
		first, second := "-", "-"
		if table.first[i].used {
			first = table.first[i].key
		}
		if table.second[i].used {
			second = table.second[i].key
		}
		fmt.Printf("%d | %s | %s\n", i, first, second)
	}
}
func (table *CuckooTable) Copy() *CuckooTable {
	result := *table
	if result.firstHash != nil {
		result.firstHash = result.firstHash.Clone()
	}
	if result.secondHash != nil {
		result.secondHash = result.secondHash.Clone()
	}
	return &result
}
func (table *CuckooTable) Assign(other *CuckooTable) { *table = *other.Copy() }

type cellState struct {
	Key  string `json:"key"`
	Used bool   `json:"used"`
}
type cuckooState struct {
	Type   string      `json:"type"`
	First  []cellState `json:"first"`
	Second []cellState `json:"second"`
}

func (table *CuckooTable) Save(filename string) error {
	state := cuckooState{Type: "cuckoo", First: make([]cellState, 11), Second: make([]cellState, 11)}
	for i := range table.first {
		if table.first[i].used {
			state.First[i] = cellState{table.first[i].key, true}
		}
		if table.second[i].used {
			state.Second[i] = cellState{table.second[i].key, true}
		}
	}
	return saveState(filename, state)
}
func (table *CuckooTable) Load(filename string) error {
	var state cuckooState
	if err := loadState(filename, &state); err != nil {
		return err
	}
	if state.Type != "cuckoo" || len(state.First) != 11 || len(state.Second) != 11 {
		return fmt.Errorf("неверный формат кукушки")
	}
	loaded := table.Copy()
	loaded.first, loaded.second = [11]CuckooCell{}, [11]CuckooCell{}
	for which, cells := range [][]cellState{state.First, state.Second} {
		for index, cell := range cells {
			if !cell.Used {
				continue
			}
			position := loaded.hash1(cell.Key)
			if which == 1 {
				position = loaded.hash2(cell.Key)
			}
			if position != index || loaded.CContains(cell.Key) {
				return fmt.Errorf("неверная позиция или повтор ключа")
			}
			if which == 0 {
				loaded.first[index] = CuckooCell{cell.Key, true}
			} else {
				loaded.second[index] = CuckooCell{cell.Key, true}
			}
		}
	}
	*table = *loaded
	return nil
}
