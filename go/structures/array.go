package structures

import (
	"fmt"
	"math"
)

type Array struct {
	data  []string
	size  int
	mesto int
}

func NewArray(capacity int) *Array {
	array := &Array{}
	array.MInitArray(capacity)
	return array
}

func (array *Array) MInitArray(capacity int) {
	if capacity < 1 {
		capacity = 1
	}
	array.data = make([]string, capacity)
	array.size = 0
	array.mesto = capacity
}

func (array *Array) MDestroy() {
	array.data = nil
	array.size = 0
	array.mesto = 0
}

func (array *Array) MResize() {
	capacity := array.mesto * 2
	if capacity == 0 {
		capacity = 2
	}
	data := make([]string, capacity)
	copy(data, array.data[:array.size])
	array.data = data
	array.mesto = capacity
}

func (array *Array) MAddEnd(value string) {
	if array.size == array.mesto {
		array.MResize()
	}
	array.data[array.size] = value
	array.size++
}

func (array *Array) MAddIndex(value string, index int) {
	if index < 0 || index > array.size {
		fmt.Println("no")
		return
	}
	if array.size == array.mesto {
		array.MResize()
	}
	for i := array.size; i > index; i-- {
		array.data[i] = array.data[i-1]
	}
	array.data[index] = value
	array.size++
}

func (array *Array) MGet(index int) string {
	if index < 0 || index >= array.size {
		fmt.Println("no")
		return ""
	}
	return array.data[index]
}

func (array *Array) MDeleteIndex(index int) {
	if index < 0 || index >= array.size {
		fmt.Println("no")
		return
	}
	for i := index; i < array.size-1; i++ {
		array.data[i] = array.data[i+1]
	}
	array.size--
	array.data[array.size] = ""
}

func (array *Array) MReplace(value string, index int) {
	if index < 0 || index >= array.size {
		fmt.Println("no")
		return
	}
	array.data[index] = value
}

func (array *Array) MLength() int { return array.size }

func (array *Array) MPrint() {
	for i := 0; i < array.size; i++ {
		fmt.Print(array.data[i], " ")
	}
	fmt.Println()
}

func (array *Array) MPrintAddresses() {
	for i := 0; i < array.size; i++ {
		fmt.Printf("[%d] %s | адрес: %p\n", i, array.data[i], &array.data[i])
	}
	fmt.Printf("size = %d mesto = %d\n\n", array.size, array.mesto)
}

func (array *Array) Copy() *Array {
	result := &Array{size: array.size, mesto: array.mesto}
	if array.mesto > 0 {
		result.data = make([]string, array.mesto)
		copy(result.data, array.data)
	}
	return result
}

func (array *Array) Assign(other *Array) { *array = *other.Copy() }

type arrayState struct {
	Type     string   `json:"type"`
	Capacity int      `json:"capacity"`
	Values   []string `json:"values"`
}

func (array *Array) Save(filename string) error {
	values := make([]string, array.size)
	copy(values, array.data[:array.size])
	return saveState(filename, arrayState{"array", array.mesto, values})
}

func (array *Array) Load(filename string) error {
	var state arrayState
	if err := loadState(filename, &state); err != nil {
		return err
	}
	if state.Type != "array" || state.Capacity < len(state.Values) || state.Capacity < 0 || state.Capacity > math.MaxInt32 {
		return fmt.Errorf("неверный формат массива")
	}
	loaded := Array{size: len(state.Values), mesto: state.Capacity}
	if loaded.mesto > 0 {
		loaded.data = make([]string, loaded.mesto)
		copy(loaded.data, state.Values)
	}
	*array = loaded
	return nil
}
