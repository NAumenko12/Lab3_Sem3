package structures

import (
	"strconv"
	"testing"
)

var benchmarkResult any

func BenchmarkStructures(b *testing.B) {
	cases := []struct {
		name   string
		action func()
	}{
		{"Array1000", func() {
			a := NewArray(1000)
			for i := 0; i < 1000; i++ {
				a.MAddEnd("block")
			}
			benchmarkResult = a
		}},
		{"FList1000", func() {
			a := NewFList()
			for i := 0; i < 1000; i++ {
				a.FAddEnd("block")
			}
			benchmarkResult = a
		}},
		{"LList1000", func() {
			a := NewLList()
			for i := 0; i < 1000; i++ {
				a.LAddEnd("block")
			}
			benchmarkResult = a
		}},
		{"Stack1000", func() {
			a := NewStack()
			for i := 0; i < 1000; i++ {
				a.SPush("block")
			}
			benchmarkResult = a
		}},
		{"Queue1000", func() {
			a := NewQueue1()
			for i := 0; i < 1000; i++ {
				a.Q1Insert("block")
			}
			benchmarkResult = a
		}},
		{"Hash1000", func() {
			a := NewHashTable(1000)
			for i := 0; i < 1000; i++ {
				a.HInsert(strconv.Itoa(i), "value")
			}
			benchmarkResult = a
		}},
		{"Tree1000", func() {
			a := NewTree()
			for i := 0; i < 1000; i++ {
				a.Add(i)
			}
			benchmarkResult = a
		}},
		{"Cuckoo3", func() {
			a := NewCuckooTable()
			for _, key := range []string{"a", "l", "w"} {
				if !a.CInsert(key) {
					panic("ошибка вставки")
				}
			}
			benchmarkResult = a
		}},
	}
	for _, test := range cases {
		b.Run(test.name, func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				test.action()
			}
		})
	}
}
