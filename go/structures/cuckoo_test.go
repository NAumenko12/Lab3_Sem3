package structures

import (
	"github.com/stretchr/testify/require"
	"lab3sem3/internal/testutil"
	"strconv"
	"testing"
)

func TestCuckooDisplacementAndRollback(t *testing.T) {
	h := NewCuckooTableWithHashes(nil, nil)
	for _, key := range []string{"a", "l", "w"} {
		require.True(t, h.CInsert(key))
	}
	require.True(t, h.CInsert("l"))
	require.False(t, h.CContains("not found"))
	copy, assigned := h.Copy(), NewCuckooTable()
	assigned.Assign(h)
	h.CDelete("w")
	h.CDelete("l")
	h.CDelete("missing")
	require.False(t, h.CContains("w"))
	require.False(t, h.CContains("l"))
	require.True(t, h.CContains("a"))
	require.True(t, copy.CContains("w"))
	require.True(t, assigned.CContains("l"))
	assigned.Assign(assigned)
	require.Contains(t, testutil.Capture(t, copy.CPrint), "Индекс | первая таблица")
	accepted := []string{}
	rejected := false
	var zero CuckooTable
	require.True(t, zero.CInsert("zero"))
	require.True(t, zero.Copy().CContains("zero"))
	h = NewCuckooTable()
	for i := 0; i < 100; i++ {
		key := strconv.Itoa(i)
		if !h.CInsert(key) {
			require.False(t, h.CContains(key))
			rejected = true
			break
		}
		accepted = append(accepted, key)
	}
	require.True(t, rejected)
	for _, key := range accepted {
		require.True(t, h.CContains(key))
	}
}
