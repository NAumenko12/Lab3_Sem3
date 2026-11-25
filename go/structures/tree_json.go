package structures

import "fmt"

type treeRecord struct {
	Value int         `json:"value"`
	Red   bool        `json:"red"`
	Left  *treeRecord `json:"left"`
	Right *treeRecord `json:"right"`
}
type treeState struct {
	Type string      `json:"type"`
	Root *treeRecord `json:"root"`
}

func treeToRecord(node *Node) *treeRecord {
	if node == nil {
		return nil
	}
	return &treeRecord{node.data, node.red, treeToRecord(node.left), treeToRecord(node.right)}
}
func recordToTree(node *treeRecord) *Node {
	if node == nil {
		return nil
	}
	return &Node{node.Value, node.Red, recordToTree(node.Left), recordToTree(node.Right)}
}
func checkTree(node, lower, upper *Node) int {
	if node == nil {
		return 0
	}
	if lower != nil && node.data <= lower.data || upper != nil && node.data >= upper.data {
		return -1
	}
	if node.red && (isRed(node.left) || isRed(node.right)) {
		return -1
	}
	left, right := checkTree(node.left, lower, node), checkTree(node.right, node, upper)
	if left < 0 || right < 0 || left != right {
		return -1
	}
	if node.red {
		return left
	}
	return left + 1
}
func (tree *Tree) Save(filename string) error {
	return saveState(filename, treeState{"tree", treeToRecord(tree.root)})
}
func (tree *Tree) Load(filename string) error {
	var state treeState
	if err := loadState(filename, &state); err != nil {
		return err
	}
	if state.Type != "tree" {
		return fmt.Errorf("неверный формат дерева")
	}
	root := recordToTree(state.Root)
	if isRed(root) || checkTree(root, nil, nil) < 0 {
		return fmt.Errorf("нарушены свойства красно-чёрного дерева")
	}
	tree.root = root
	return nil
}
