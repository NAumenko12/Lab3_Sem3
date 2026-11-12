package structures

import (
	"github.com/stretchr/testify/require"
	"lab3sem3/internal/testutil"
	"path/filepath"
	"testing"
)

func TestArrayOperations(t *testing.T) {
	a := NewArray(1)
	a.MAddEnd("a")
	a.MAddIndex("b", 1)
	a.MAddIndex("x", 0)
	require.Equal(t, 3, a.MLength())
	require.Equal(t, "b", a.MGet(2))
	a.MReplace("c", 1)
	a.MDeleteIndex(0)
	require.Equal(t, "c", a.MGet(0))
	require.Equal(t, "c b \n", testutil.Capture(t, a.MPrint))
	require.Contains(t, testutil.Capture(t, a.MPrintAddresses), "size = 2")
	copy, assigned := a.Copy(), NewArray(0)
	assigned.Assign(a)
	a.MReplace("changed", 0)
	require.Equal(t, "c", copy.MGet(0))
	require.Equal(t, "c", assigned.MGet(0))
	assigned.Assign(assigned)
	require.Equal(t, "c", assigned.MGet(0))
}

func TestArrayInvalidAndDestroyed(t *testing.T) {
	var a Array
	a.MAddEnd("saved")
	testutil.Capture(t, func() {
		a.MAddIndex("bad", -1)
		a.MAddIndex("bad", 9)
		a.MDeleteIndex(-1)
		a.MDeleteIndex(9)
		a.MReplace("bad", 2)
		require.Empty(t, a.MGet(9))
	})
	require.Equal(t, 1, a.MLength())
	require.Equal(t, "saved", a.MGet(0))
	a.MDestroy()
	require.Zero(t, a.Copy().MLength())
	filename := filepath.Join(t.TempDir(), "array.json")
	require.NoError(t, a.Save(filename))
	require.NoError(t, a.Load(filename))
	a.MAddEnd("again")
	require.Equal(t, "again", a.MGet(0))
	a.MInitArray(0)
	require.Zero(t, a.MLength())
}
