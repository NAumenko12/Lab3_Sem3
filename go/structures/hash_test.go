package structures

import (
	"github.com/stretchr/testify/require"
	"lab3sem3/internal/testutil"
	"strconv"
	"testing"
)

func TestHashChainsGrowthAndCopies(t *testing.T) {
	h := NewHashTable(32)
	for _, key := range []string{"Aa", "BB", "C#"} {
		h.HInsert(key, key)
	}
	h.HInsert("Aa", "updated")
	require.Equal(t, 3, h.HLength())
	value, found := h.HGet("Aa")
	require.True(t, found)
	require.Equal(t, "updated", value)
	require.Contains(t, testutil.Capture(t, h.HPrint), "Aa -> updated")
	require.Contains(t, testutil.Capture(t, h.HPrintAddresses), "size = 3")
	copy, assigned := h.Copy(), NewHashTable(8)
	assigned.Assign(h)
	h.HDelete("BB")
	h.HDelete("C#")
	h.HDelete("Aa")
	h.HDelete("missing")
	require.Zero(t, h.HLength())
	require.True(t, copy.HContains("BB"))
	require.True(t, assigned.HContains("C#"))
	h.HInitTable(2)
	for i := 0; i < 100; i++ {
		h.HInsert(strconv.Itoa(i), "value")
	}
	for i := 0; i < 100; i++ {
		value, found = h.HGet(strconv.Itoa(i))
		require.True(t, found)
		require.Equal(t, "value", value)
	}
	h.Assign(h)
	require.Equal(t, 100, h.HLength())
	h.HClear()
	require.Zero(t, h.HLength())
	h.HDestroy()
	require.False(t, h.HContains("x"))
	h.HDelete("x")
	require.Zero(t, h.Copy().HLength())
	h.HInsert("again", "yes")
	require.True(t, h.HContains("again"))
	require.False(t, h.HContains("missing"))
	var zero HashTable
	require.Zero(t, zero.Copy().HLength())
	zero.HInsert("a", "b")
	require.True(t, zero.HContains("a"))
}
func TestHashStrategies(t *testing.T) {
	for _, f := range []HashFunction{PolynomialHash{}, CuckooHash1{}, CuckooHash2{}} {
		copy := f.Clone()
		require.Equal(t, f.HHash("Науменко", 11), copy.HHash("Науменко", 11))
		require.Equal(t, 0, f.HHash("", 11))
		require.Less(t, f.HHash("abc", 11), 11)
	}
	for i, key := range []string{"a", "l", "w"} {
		require.Equal(t, 9, (CuckooHash1{}).HHash(key, 11))
		require.Equal(t, 6+i, (CuckooHash2{}).HHash(key, 11))
	}
	h := NewHashTableWithHash(0, CuckooHash1{})
	h.HInsert("key", "value")
	require.True(t, h.Copy().HContains("key"))
}
