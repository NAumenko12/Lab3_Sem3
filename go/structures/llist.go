package structures

import "fmt"

type LList struct {
	head *LNode
	tail *LNode
}

func NewLList() *LList              { return &LList{} }
func (list *LList) GetHead() *LNode { return list.head }
func (list *LList) GetTail() *LNode { return list.tail }
func (list *LList) LInitList()      { list.head = nil; list.tail = nil }

func (list *LList) LAddFirst(value string) {
	node := &LNode{Chislo: value, Next: list.head}
	if list.head != nil {
		list.head.Prev = node
	} else {
		list.tail = node
	}
	list.head = node
}

func (list *LList) LAddEnd(value string) {
	node := &LNode{Chislo: value, Prev: list.tail}
	if list.tail != nil {
		list.tail.Next = node
	} else {
		list.head = node
	}
	list.tail = node
}

func (list *LList) LAddAfter(current *LNode, value string) {
	if current == nil {
		fmt.Println("no")
		return
	}
	node := &LNode{value, current.Next, current}
	if current.Next != nil {
		current.Next.Prev = node
	} else {
		list.tail = node
	}
	current.Next = node
}

func (list *LList) LAddBefore(current *LNode, value string) {
	if current == nil {
		fmt.Println("no")
		return
	}
	node := &LNode{value, current, current.Prev}
	if current.Prev != nil {
		current.Prev.Next = node
	} else {
		list.head = node
	}
	current.Prev = node
}

func (list *LList) LAddIndex(value string, index int) {
	if index < 0 {
		fmt.Println("no")
		return
	}
	if index == 0 {
		list.LAddFirst(value)
		return
	}
	current := list.head
	for i := 0; i < index-1 && current != nil; i++ {
		current = current.Next
	}
	if current == nil {
		fmt.Println("no")
		return
	}
	list.LAddAfter(current, value)
}

func (list *LList) LDeleteFirst() {
	if list.head == nil {
		fmt.Println("no")
		return
	}
	list.head = list.head.Next
	if list.head != nil {
		list.head.Prev = nil
	} else {
		list.tail = nil
	}
}

func (list *LList) LDeleteEnd() {
	if list.tail == nil {
		fmt.Println("no")
		return
	}
	list.tail = list.tail.Prev
	if list.tail != nil {
		list.tail.Next = nil
	} else {
		list.head = nil
	}
}

func (list *LList) LDeleteAfter(node *LNode) {
	if node == nil || node.Next == nil {
		fmt.Println("no")
		return
	}
	node.Next = node.Next.Next
	if node.Next != nil {
		node.Next.Prev = node
	} else {
		list.tail = node
	}
}

func (list *LList) LDeleteBefore(node *LNode) {
	if node == nil || node.Prev == nil {
		fmt.Println("no")
		return
	}
	node.Prev = node.Prev.Prev
	if node.Prev != nil {
		node.Prev.Next = node
	} else {
		list.head = node
	}
}

func (list *LList) LDeleteNode(node *LNode) {
	if node == nil {
		fmt.Println("no")
		return
	}
	if node.Prev != nil {
		node.Prev.Next = node.Next
	} else {
		list.head = node.Next
	}
	if node.Next != nil {
		node.Next.Prev = node.Prev
	} else {
		list.tail = node.Prev
	}
}

func (list *LList) LDeleteIndex(index int) {
	if index < 0 {
		fmt.Println("no")
		return
	}
	node := list.head
	for i := 0; i < index && node != nil; i++ {
		node = node.Next
	}
	list.LDeleteNode(node)
}

func (list *LList) LDeleteValue(value string) {
	node := list.head
	for node != nil && node.Chislo != value {
		node = node.Next
	}
	if node != nil {
		list.LDeleteNode(node)
	}
}

func (list *LList) LPrint(node *LNode) {
	for node != nil {
		fmt.Println(node.Chislo)
		node = node.Next
	}
}
func (list *LList) LPrintBac(node *LNode) {
	for node != nil {
		fmt.Println(node.Chislo)
		node = node.Prev
	}
}

func (list *LList) LPrintAddresses(node *LNode) {
	for node != nil {
		fmt.Printf("%s | адрес: %p | next: %p | prev: %p\n", node.Chislo, node, node.Next, node.Prev)
		node = node.Next
	}
	fmt.Println()
}

func (list *LList) LFind(node *LNode, value string) {
	for node != nil {
		if node.Chislo == value {
			fmt.Printf("%s найден, адрес: %p\n", value, node)
			return
		}
		node = node.Next
	}
	fmt.Printf("%s не найден\n", value)
}

func (list *LList) Copy() *LList {
	result := NewLList()
	for node := list.head; node != nil; node = node.Next {
		result.LAddEnd(node.Chislo)
	}
	return result
}

func (list *LList) Assign(other *LList) { *list = *other.Copy() }

func (list *LList) Save(filename string) error {
	values := make([]string, 0)
	for node := list.head; node != nil; node = node.Next {
		values = append(values, node.Chislo)
	}
	return saveState(filename, listState{"llist", values})
}

func (list *LList) Load(filename string) error {
	var state listState
	if err := loadState(filename, &state); err != nil {
		return err
	}
	if state.Type != "llist" {
		return fmt.Errorf("неверный формат двусвязного списка")
	}
	loaded := LList{}
	for _, value := range state.Values {
		loaded.LAddEnd(value)
	}
	*list = loaded
	return nil
}
