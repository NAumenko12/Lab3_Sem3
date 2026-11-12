package structures

import "fmt"

type Queue1 struct {
	list FList
	tail *ListNode
}

func NewQueue1() *Queue1                { return &Queue1{} }
func (queue *Queue1) Q1InitQueue()      { queue.list.FInitList(); queue.tail = nil }
func (queue *Queue1) Q1PrintQueue()     { queue.list.FPrint(queue.list.head) }
func (queue *Queue1) Q1PrintAddresses() { queue.list.FPrintAddresses(queue.list.head) }
func (queue *Queue1) Q1Insert(value string) {
	if queue.tail == nil {
		queue.list.FAddFirst(value)
		queue.tail = queue.list.head
		return
	}
	queue.list.FAddAfter(queue.tail, value)
	queue.tail = queue.tail.Next
}
func (queue *Queue1) Q1Delete() {
	queue.list.FDeleteFirst()
	if queue.list.head == nil {
		queue.tail = nil
	}
}
func (queue *Queue1) Q1Front() string {
	if queue.list.head == nil {
		fmt.Println("no")
		return ""
	}
	return queue.list.head.Chislo
}
func (queue *Queue1) Q1Back() string {
	if queue.tail == nil {
		fmt.Println("no")
		return ""
	}
	return queue.tail.Chislo
}
func (queue *Queue1) findTail() {
	queue.tail = queue.list.head
	if queue.tail != nil {
		for queue.tail.Next != nil {
			queue.tail = queue.tail.Next
		}
	}
}
func (queue *Queue1) Copy() *Queue1 {
	result := &Queue1{list: *queue.list.Copy()}
	result.findTail()
	return result
}
func (queue *Queue1) Assign(other *Queue1)       { *queue = *other.Copy() }
func (queue *Queue1) Save(filename string) error { return queue.list.Save(filename) }
func (queue *Queue1) Load(filename string) error {
	if err := queue.list.Load(filename); err != nil {
		return err
	}
	queue.findTail()
	return nil
}
