package structures

import "fmt"

type FList struct{ head *ListNode }

func NewFList() *FList                 { return &FList{} }
func (list *FList) GetHead() *ListNode { return list.head }
func (list *FList) FInitList()         { list.head = nil }

func (list *FList) FAddFirst(value string) { list.head = &ListNode{value, list.head} }

func (list *FList) FAddEnd(value string) {
	node := &ListNode{Chislo: value}
	if list.head == nil {
		list.head = node
		return
	}
	current := list.head
	for current.Next != nil {
		current = current.Next
	}
	current.Next = node
}

func (list *FList) FAddAfter(node *ListNode, value string) {
	if node == nil {
		fmt.Println("no")
		return
	}
	node.Next = &ListNode{value, node.Next}
}

func (list *FList) FAddBefore(target *ListNode, value string) {
	if target == nil {
		fmt.Println("no")
		return
	}
	if target == list.head {
		list.FAddFirst(value)
		return
	}
	current := list.head
	for current != nil && current.Next != target {
		current = current.Next
	}
	if current == nil {
		fmt.Println("no")
		return
	}
	current.Next = &ListNode{value, target}
}

func (list *FList) FAddIndex(value string, index int) {
	if index < 0 {
		fmt.Println("no")
		return
	}
	if index == 0 {
		list.FAddFirst(value)
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
	current.Next = &ListNode{value, current.Next}
}

func (list *FList) FDeleteFirst() {
	if list.head == nil {
		fmt.Println("no")
		return
	}
	list.head = list.head.Next
}

func (list *FList) FDeleteAfter(node *ListNode) {
	if node == nil || node.Next == nil {
		fmt.Println("no")
		return
	}
	node.Next = node.Next.Next
}

func (list *FList) FDeleteEnd() {
	if list.head == nil {
		fmt.Println("no")
		return
	}
	if list.head.Next == nil {
		list.FDeleteFirst()
		return
	}
	current := list.head
	for current.Next.Next != nil {
		current = current.Next
	}
	list.FDeleteAfter(current)
}

func (list *FList) FDeleteBefore(target *ListNode) {
	if list.head == nil || target == nil || target == list.head {
		fmt.Println("no")
		return
	}
	if list.head.Next == target {
		list.FDeleteFirst()
		return
	}
	current := list.head
	for current.Next != nil && current.Next.Next != target {
		current = current.Next
	}
	if current.Next == nil {
		fmt.Println("no")
		return
	}
	list.FDeleteAfter(current)
}

func (list *FList) FDeleteIndex(index int) {
	if index < 0 {
		fmt.Println("no")
		return
	}
	if index == 0 {
		list.FDeleteFirst()
		return
	}
	current := list.head
	for i := 0; i < index-1 && current != nil; i++ {
		current = current.Next
	}
	if current == nil || current.Next == nil {
		fmt.Println("no")
		return
	}
	list.FDeleteAfter(current)
}

func (list *FList) FDeleteValue(value string) {
	if list.head == nil {
		return
	}
	if list.head.Chislo == value {
		list.FDeleteFirst()
		return
	}
	current := list.head
	for current.Next != nil && current.Next.Chislo != value {
		current = current.Next
	}
	if current.Next != nil {
		list.FDeleteAfter(current)
	}
}

func (list *FList) FPrint(node *ListNode) {
	for node != nil {
		fmt.Println(node.Chislo)
		node = node.Next
	}
}

func (list *FList) FPrintBack(node *ListNode) {
	if node == nil {
		return
	}
	list.FPrintBack(node.Next)
	fmt.Println(node.Chislo)
}

func (list *FList) FPrintAddresses(node *ListNode) {
	for node != nil {
		fmt.Printf("Узел: %p, chislo: %s, next: %p\n", node, node.Chislo, node.Next)
		node = node.Next
	}
}

func (list *FList) FFind(node *ListNode, value string) {
	for node != nil {
		if node.Chislo == value {
			fmt.Printf("%s найден, адрес: %p\n", value, node)
			return
		}
		node = node.Next
	}
	fmt.Printf("%s не найден\n", value)
}

func (list *FList) Copy() *FList {
	result := NewFList()
	for node := list.head; node != nil; node = node.Next {
		result.FAddEnd(node.Chislo)
	}
	return result
}

func (list *FList) Assign(other *FList) { *list = *other.Copy() }

func (list *FList) Save(filename string) error {
	values := make([]string, 0)
	for node := list.head; node != nil; node = node.Next {
		values = append(values, node.Chislo)
	}
	return saveState(filename, listState{"flist", values})
}

func (list *FList) Load(filename string) error {
	var state listState
	if err := loadState(filename, &state); err != nil {
		return err
	}
	if state.Type != "flist" {
		return fmt.Errorf("неверный формат односвязного списка")
	}
	loaded := FList{}
	for _, value := range state.Values {
		loaded.FAddEnd(value)
	}
	*list = loaded
	return nil
}
