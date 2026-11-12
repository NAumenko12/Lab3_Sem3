package structures

type ListNode struct {
	Chislo string
	Next   *ListNode
}

type LNode struct {
	Chislo string
	Next   *LNode
	Prev   *LNode
}

type HNode struct {
	Key   string
	Value string
	Next  *HNode
}

type Node struct {
	data  int
	red   bool
	left  *Node
	right *Node
}
