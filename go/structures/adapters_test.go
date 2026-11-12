package structures

import (
	"github.com/stretchr/testify/require"
	"lab3sem3/internal/testutil"
	"path/filepath"
	"testing"
)

func TestStackAdapter(t *testing.T) {
	s := NewStack()
	require.True(t, s.SIsEmptyStack())
	s.SPush("a")
	s.SPush("b")
	copy, assigned := s.Copy(), NewStack()
	assigned.Assign(s)
	require.Equal(t, "b\na\n", testutil.Capture(t, func() { s.SPrint(s.GetHead()) }))
	require.Contains(t, testutil.Capture(t, s.SPrintAddresses), "chislo: b")
	s.SPop()
	require.Equal(t, "a", s.STop())
	require.Equal(t, "b", copy.STop())
	require.Equal(t, "b", assigned.STop())
	require.NotSame(t, copy.GetHead(), assigned.GetHead())
	file := filepath.Join(t.TempDir(), "stack.json")
	require.NoError(t, s.Save(file))
	require.NoError(t, assigned.Load(file))
	require.Equal(t, "a", assigned.STop())
	s.SInitStack()
	require.True(t, s.SIsEmptyStack())
	testutil.Capture(t, func() { s.SPop(); require.Empty(t, s.STop()) })
}
func TestQueueAdapterAndTailAfterLoad(t *testing.T) {
	q := NewQueue1()
	q.Q1Insert("a")
	q.Q1Insert("b")
	copy, assigned := q.Copy(), NewQueue1()
	assigned.Q1Insert("old")
	assigned.Assign(q)
	q.Q1Delete()
	copy.Q1Insert("c")
	require.Equal(t, "b", q.Q1Front())
	require.Equal(t, "c", copy.Q1Back())
	require.Equal(t, "a", assigned.Q1Front())
	require.Equal(t, "a\nb\n", testutil.Capture(t, assigned.Q1PrintQueue))
	require.Contains(t, testutil.Capture(t, assigned.Q1PrintAddresses), "chislo: a")
	file := filepath.Join(t.TempDir(), "queue.json")
	require.NoError(t, copy.Save(file))
	require.NoError(t, q.Load(file))
	q.Q1Insert("d")
	require.Equal(t, "d", q.Q1Back())
	q.Assign(q)
	require.Equal(t, "a", q.Q1Front())
	q.Q1InitQueue()
	testutil.Capture(t, func() { q.Q1Delete(); require.Empty(t, q.Q1Front()); require.Empty(t, q.Q1Back()) })
	empty := NewQueue1().Copy()
	require.Nil(t, empty.tail)
	require.NoError(t, empty.Save(file))
	require.NoError(t, q.Load(file))
	q.Q1Insert("new")
	require.Equal(t, "new", q.Q1Back())
}
