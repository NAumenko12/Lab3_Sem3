package structures

import "fmt"

type Tree struct{ root *Node }

func NewTree() *Tree              { return &Tree{} }
func (tree *Tree) GetRoot() *Node { return tree.root }
func (tree *Tree) InitTree()      { tree.root = nil }
func isRed(node *Node) bool       { return node != nil && node.red }

func rotateLeft(node **Node) {
	root := *node
	parent := root.right
	root.right = parent.left
	parent.left = root
	*node = parent
}
func rotateRight(node **Node) {
	root := *node
	parent := root.left
	root.left = parent.right
	parent.right = root
	*node = parent
}
func balance(node **Node) {
	root := *node
	if root == nil || isRed(root) {
		return
	}
	if isRed(root.left) && (isRed(root.left.left) || isRed(root.left.right)) {
		if isRed(root.right) {
			root.left.red = false
			root.right.red = false
			root.red = true
		} else {
			if isRed(root.left.right) {
				rotateLeft(&root.left)
			}
			rotateRight(node)
			(*node).red = false
			(*node).right.red = true
		}
	} else if isRed(root.right) && (isRed(root.right.left) || isRed(root.right.right)) {
		if isRed(root.left) {
			root.left.red = false
			root.right.red = false
			root.red = true
		} else {
			if isRed(root.right.left) {
				rotateRight(&root.right)
			}
			rotateLeft(node)
			(*node).red = false
			(*node).left.red = true
		}
	}
}
func addRecursive(node **Node, value int) {
	root := *node
	if value < root.data {
		if root.left == nil {
			root.left = &Node{data: value, red: true}
		} else {
			addRecursive(&root.left, value)
		}
	} else if value > root.data {
		if root.right == nil {
			root.right = &Node{data: value, red: true}
		} else {
			addRecursive(&root.right, value)
		}
	} else {
		return
	}
	balance(node)
}
func (tree *Tree) Add(value int) {
	if tree.root == nil {
		tree.root = &Node{data: value}
		return
	}
	addRecursive(&tree.root, value)
	tree.root.red = false
}
func balanceDeleteLeft(node **Node, blackLost *bool) {
	if !*blackLost {
		return
	}
	root := *node
	if isRed(root.right) {
		root.right.red = false
		root.red = true
		rotateLeft(node)
		balanceDeleteLeft(&(*node).left, blackLost)
		return
	}
	brother := root.right
	if !isRed(brother.left) && !isRed(brother.right) {
		brother.red = true
		if isRed(root) {
			root.red = false
			*blackLost = false
		}
		return
	}
	if !isRed(brother.right) {
		brother.left.red = false
		brother.red = true
		rotateRight(&root.right)
	}
	root.right.red = root.red
	root.red = false
	root.right.right.red = false
	rotateLeft(node)
	*blackLost = false
}
func balanceDeleteRight(node **Node, blackLost *bool) {
	if !*blackLost {
		return
	}
	root := *node
	if isRed(root.left) {
		root.left.red = false
		root.red = true
		rotateRight(node)
		balanceDeleteRight(&(*node).right, blackLost)
		return
	}
	brother := root.left
	if !isRed(brother.left) && !isRed(brother.right) {
		brother.red = true
		if isRed(root) {
			root.red = false
			*blackLost = false
		}
		return
	}
	if !isRed(brother.left) {
		brother.right.red = false
		brother.red = true
		rotateLeft(&root.left)
	}
	root.left.red = root.red
	root.red = false
	root.left.left.red = false
	rotateRight(node)
	*blackLost = false
}
func deleteRecursive(node **Node, value int, blackLost *bool) {
	root := *node
	if root == nil {
		*blackLost = false
		return
	}
	if value < root.data {
		deleteRecursive(&root.left, value, blackLost)
		balanceDeleteLeft(node, blackLost)
		return
	}
	if value > root.data {
		deleteRecursive(&root.right, value, blackLost)
		balanceDeleteRight(node, blackLost)
		return
	}
	if root.left != nil && root.right != nil {
		replacement := root.right
		for replacement.left != nil {
			replacement = replacement.left
		}
		root.data = replacement.data
		deleteRecursive(&root.right, replacement.data, blackLost)
		balanceDeleteRight(node, blackLost)
		return
	}
	if root.left != nil {
		*node = root.left
	} else {
		*node = root.right
	}
	if root.red {
		*blackLost = false
	} else if isRed(*node) {
		(*node).red = false
		*blackLost = false
	} else {
		*blackLost = true
	}
}
func (tree *Tree) Delete(value int) {
	blackLost := false
	deleteRecursive(&tree.root, value, &blackLost)
	if tree.root != nil {
		tree.root.red = false
	}
}
func (tree *Tree) PrintPryam(node *Node) {
	if node == nil {
		return
	}
	fmt.Print(node.data)
	if node.red {
		fmt.Print(" К ")
	} else {
		fmt.Print(" Ч ")
	}
	tree.PrintPryam(node.left)
	tree.PrintPryam(node.right)
}
func (tree *Tree) PrintAddresses(node *Node) {
	if node == nil {
		return
	}
	fmt.Printf("Значение: %d | адрес: %p | left: %p | right: %p\n", node.data, node, node.left, node.right)
	tree.PrintAddresses(node.left)
	tree.PrintAddresses(node.right)
}
func (tree *Tree) Find(node *Node, value int) {
	for node != nil {
		if node.data == value {
			fmt.Printf("%d найден, адрес: %p\n", value, node)
			return
		}
		if value < node.data {
			node = node.left
		} else {
			node = node.right
		}
	}
	fmt.Printf("%d не найден\n", value)
}
func copyNode(node *Node) *Node {
	if node == nil {
		return nil
	}
	return &Node{data: node.data, red: node.red, left: copyNode(node.left), right: copyNode(node.right)}
}
func (tree *Tree) Copy() *Tree        { return &Tree{root: copyNode(tree.root)} }
func (tree *Tree) Assign(other *Tree) { tree.root = copyNode(other.root) }
