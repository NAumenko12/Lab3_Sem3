package structures

import (
	"github.com/stretchr/testify/require"
	"lab3sem3/internal/testutil"
	"testing"
)

func fValues(list *FList) []string {
	values := make([]string, 0)
	for n := list.GetHead(); n != nil; n = n.Next {
		values = append(values, n.Chislo)
	}
	return values
}
func lValues(t *testing.T, list *LList) []string {
	values := make([]string, 0)
	var previous *LNode
	for n := list.GetHead(); n != nil; n = n.Next {
		require.Same(t, previous, n.Prev)
		values = append(values, n.Chislo)
		previous = n
	}
	require.Same(t, previous, list.GetTail())
	return values
}
func TestFListOrderCopyAndRemoval(t *testing.T) {
	l := NewFList()
	l.FAddEnd("b")
	l.FAddFirst("a")
	l.FAddAfter(l.head.Next, "d")
	l.FAddBefore(l.head.Next.Next, "c")
	l.FAddBefore(l.head, "start")
	l.FAddIndex("end", 5)
	require.Equal(t, []string{"start", "a", "b", "c", "d", "end"}, fValues(l))
	require.Equal(t, "end\nd\nc\nb\na\nstart\n", testutil.Capture(t, func() { l.FPrintBack(l.head) }))
	require.Equal(t, "start\na\nb\nc\nd\nend\n", testutil.Capture(t, func() { l.FPrint(l.head) }))
	require.Contains(t, testutil.Capture(t, func() { l.FFind(l.head, "c"); l.FFind(l.head, "missing"); l.FPrintAddresses(l.head) }), "missing не найден")
	copy, assigned := l.Copy(), NewFList()
	assigned.Assign(l)
	l.FDeleteBefore(l.head.Next.Next)
	l.FDeleteAfter(l.head)
	l.FDeleteEnd()
	l.FDeleteIndex(1)
	l.FDeleteValue("d")
	l.FDeleteValue("missing")
	l.FDeleteValue("start")
	require.Empty(t, fValues(l))
	require.Len(t, fValues(copy), 6)
	require.Equal(t, fValues(copy), fValues(assigned))
	require.NotSame(t, copy.head, assigned.head)
	assigned.Assign(assigned)
	assigned.FInitList()
	require.Nil(t, assigned.head)
}
func TestLListOrderCopyAndLinks(t *testing.T) {
	l := NewLList()
	l.LAddEnd("b")
	l.LAddFirst("a")
	l.LAddAfter(l.tail, "d")
	l.LAddBefore(l.tail, "c")
	l.LAddBefore(l.head, "start")
	l.LAddIndex("end", 5)
	require.Equal(t, []string{"start", "a", "b", "c", "d", "end"}, lValues(t, l))
	require.Equal(t, "end\nd\nc\nb\na\nstart\n", testutil.Capture(t, func() { l.LPrintBac(l.tail) }))
	require.Equal(t, "start\na\nb\nc\nd\nend\n", testutil.Capture(t, func() { l.LPrint(l.head) }))
	require.Contains(t, testutil.Capture(t, func() { l.LFind(l.head, "c"); l.LFind(l.head, "missing"); l.LPrintAddresses(l.head) }), "missing не найден")
	copy, assigned := l.Copy(), NewLList()
	assigned.Assign(l)
	l.LDeleteBefore(l.head.Next.Next)
	l.LDeleteAfter(l.head)
	l.LDeleteEnd()
	l.LDeleteIndex(1)
	l.LDeleteValue("d")
	l.LDeleteValue("missing")
	l.LDeleteValue("start")
	require.Empty(t, lValues(t, l))
	require.Len(t, lValues(t, copy), 6)
	require.Equal(t, lValues(t, copy), lValues(t, assigned))
	require.NotSame(t, copy.tail, assigned.tail)
	assigned.Assign(assigned)
	assigned.LInitList()
	require.Nil(t, assigned.tail)
}
