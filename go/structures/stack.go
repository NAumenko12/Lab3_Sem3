package structures

import "fmt"

type Stack struct{ list FList }

func NewStack() *Stack                     { return &Stack{} }
func (stack *Stack) SInitStack()           { stack.list.FInitList() }
func (stack *Stack) SIsEmptyStack() bool   { return stack.list.head == nil }
func (stack *Stack) SPush(value string)    { stack.list.FAddFirst(value) }
func (stack *Stack) SPop()                 { stack.list.FDeleteFirst() }
func (stack *Stack) GetHead() *ListNode    { return stack.list.head }
func (stack *Stack) SPrint(node *ListNode) { stack.list.FPrint(node) }
func (stack *Stack) SPrintAddresses()      { stack.list.FPrintAddresses(stack.list.head) }
func (stack *Stack) STop() string {
	if stack.SIsEmptyStack() {
		fmt.Println("no")
		return ""
	}
	return stack.list.head.Chislo
}
func (stack *Stack) Copy() *Stack               { return &Stack{list: *stack.list.Copy()} }
func (stack *Stack) Assign(other *Stack)        { *stack = *other.Copy() }
func (stack *Stack) Save(filename string) error { return stack.list.Save(filename) }
func (stack *Stack) Load(filename string) error { return stack.list.Load(filename) }
