#include "../serialization/file_archive.h"
#include <stdexcept>
#include "black_red_tree.h"
#include <iostream>
using namespace std;

Tree::Tree() : root(nullptr) {}

void Tree::InitTree() {
    Clear(root);
}

Tree::~Tree() {
    Clear(root);
}

void Tree::Clear(Node*& node) {
    if (node == nullptr) {
        return;
    }
    Clear(node->left);
    Clear(node->right);
    delete node;
    node = nullptr;
}

bool Tree::IsRed(Node* node) const {
    return node != nullptr && node->red;
}

void Tree::RotateLeft(Node*& node) {
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    node = newRoot;
}

void Tree::RotateRight(Node*& node) {
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    node = newRoot;
}
void Tree::Balance(Node*& node) {
    if (node == nullptr || IsRed(node)) {
        return;
    }
    if (IsRed(node->left) && (IsRed(node->left->left) || IsRed(node->left->right))) {
        if (IsRed(node->right)) {
            node->left->red = false;
            node->right->red = false;
            node->red = true;
        } else {
            if (IsRed(node->left->right)) {
                RotateLeft(node->left);
            }
            RotateRight(node);
            node->red = false;
            node->right->red = true;
        }
    } else if (IsRed(node->right) && (IsRed(node->right->left) || IsRed(node->right->right))) {
        if (IsRed(node->left)) {
            node->left->red = false;
            node->right->red = false;
            node->red = true;
        } else {
            if (IsRed(node->right->left)) {
                RotateRight(node->right);
            }
            RotateLeft(node);
            node->red = false;
            node->left->red = true;
        }
    }
}
void Tree::AddRecurs(Node*& node, int chislo) {
    if (chislo < node->data) {
        if (node->left == nullptr) {
            node->left = new Node(chislo, true);
        } else {
            AddRecurs(node->left, chislo);
        }
    } else if (chislo > node->data) {
        if (node->right == nullptr) {
            node->right = new Node(chislo, true);
        } else {
            AddRecurs(node->right, chislo);
        }
    } else {
        return;
    }

    Balance(node);
}

void Tree::Add(int chislo) {
    if (root == nullptr) {
        root = new Node(chislo, false);
        return;
    }
    AddRecurs(root, chislo);
    root->red = false;
}
void Tree::BalanceDeleteLeft(Node*& node, bool& blackLost) {
    if (!blackLost) {
        return;
    }
    if (IsRed(node->right)) {
        node->right->red = false;
        node->red = true;
        RotateLeft(node);
        BalanceDeleteLeft(node->left, blackLost);
        return;
    }
    Node* brother = node->right;
    if (!IsRed(brother->left) && !IsRed(brother->right)) {
        brother->red = true;
        if (IsRed(node)) {
            node->red = false;
            blackLost = false;
        }
        return;
    }
    if (!IsRed(brother->right)) {
        brother->left->red = false;
        brother->red = true;
        RotateRight(node->right);
    }
    node->right->red = node->red;
    node->red = false;
    node->right->right->red = false;
    RotateLeft(node);
    blackLost = false;
}
void Tree::BalanceDeleteRight(Node*& node, bool& blackLost) {
    if (!blackLost) {
        return;
    }
    if (IsRed(node->left)) {
        node->left->red = false;
        node->red = true;
        RotateRight(node);
        BalanceDeleteRight(node->right, blackLost);
        return;
    }
    Node* brother = node->left;
    if (!IsRed(brother->left) && !IsRed(brother->right)) {
        brother->red = true;
        if (IsRed(node)) {
            node->red = false;
            blackLost = false;
        }
        return;
    }
    if (!IsRed(brother->left)) {
        brother->right->red = false;
        brother->red = true;
        RotateLeft(node->left);
    }
    node->left->red = node->red;
    node->red = false;
    node->left->left->red = false;
    RotateRight(node);
    blackLost = false;
}
void Tree::DeleteRecurs(Node*& node, int chislo, bool& blackLost) {
    if (node == nullptr) {
        blackLost = false;
        return;
    }
    if (chislo < node->data) {
        DeleteRecurs(node->left, chislo, blackLost);
        BalanceDeleteLeft(node, blackLost);
        return;
    }
    if (chislo > node->data) {
        DeleteRecurs(node->right, chislo, blackLost);
        BalanceDeleteRight(node, blackLost);
        return;
    }
    if (node->left != nullptr && node->right != nullptr) {
        Node* rep = node->right;
        while (rep->left != nullptr) {
            rep = rep->left;
        }
        node->data = rep->data;
        DeleteRecurs(node->right, rep->data, blackLost);
        BalanceDeleteRight(node, blackLost);
        return;
    }
    Node* old = node;
    if (node->left != nullptr) {
        node = node->left;
    } else {
        node = node->right;
    }
    if (old->red) {
        blackLost = false;
    } else if (IsRed(node)) {
        node->red = false;
        blackLost = false;
    } else {
        blackLost = true;
    }
    delete old;
}

void Tree::Delete(int chislo) {
    bool blackLost = false;
    DeleteRecurs(root, chislo, blackLost);
    if (root != nullptr) {
        root->red = false;
    }
}
void Tree::PrintPryam(Node* node) const {
    if (node == nullptr) {
        return;
    }
    cout << node->data;
    if (node->red) {
        cout << " К ";
    } else {
        cout << " Ч ";
    }
    PrintPryam(node->left);
    PrintPryam(node->right);
}

void Tree::PrintAddresses(Node* node) const {
    if (node == nullptr) {
        return;
    }
    cout << "Значение: " << node->data << " | адрес: " << node << " | left: " << node->left
         << " | right: " << node->right << '\n';
    PrintAddresses(node->left);
    PrintAddresses(node->right);
}

void Tree::Find(Node* node, int chislo) const {
    while (node != nullptr) {
        if (chislo == node->data) {
            cout << chislo << " найден, адрес: " << node << "\n";
            return;
        }
        if (chislo < node->data) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    cout << chislo << " не найден\n";
}

Node* Tree::GetRoot() const {
    return root;
}
Node* Tree::CopyNode(const Node* node) {
    if (node == nullptr) {
        return nullptr;
    }
    Tree copy;
    copy.root = new Node(node->data, node->red);
    copy.root->left = CopyNode(node->left);
    copy.root->right = CopyNode(node->right);
    Node* result = copy.root;
    copy.root = nullptr;
    return result;
}

Tree::Tree(const Tree& other) : root(nullptr) {
    root = CopyNode(other.root);
}

Tree& Tree::operator=(const Tree& other) {
    if (this == &other) {
        return *this;
    }
    Tree copy(other);
    Node* old_root = root;
    root = copy.root;
    copy.root = old_root;
    return *this;
}

void Tree::SaveNode(FileWriter& file, const Node* node) const {
    file.WriteFlag(node != nullptr);
    if (node == nullptr) {
        return;
    }
    file.WriteInt(node->data);
    file.WriteFlag(node->red);
    SaveNode(file, node->left);
    SaveNode(file, node->right);
}

void Tree::Save(const string& filename) const {
    FileWriter file(filename, "tree");
    SaveNode(file, root);
    file.Finish();
}

void Tree::LoadNode(FileReader& file, Node*& node) {
    if (!file.ReadFlag()) {
        node = nullptr;
        return;
    }
    int chislo = file.ReadInt();
    bool red = file.ReadFlag();
    node = new Node(chislo, red);
    LoadNode(file, node->left);
    LoadNode(file, node->right);
}
void Tree::Load(const string& filename) {
    FileReader file(filename, "tree");
    Tree loaded;
    LoadNode(file, loaded.root);
    file.Finish();
    if ((loaded.root != nullptr && loaded.root->red) ||
        loaded.CheckTree(loaded.root, nullptr, nullptr) < 0) {
        throw runtime_error("Ошибка: нарушены свойства красно-чёрного дерева");
    }
    Node* old_root = root;
    root = loaded.root;
    loaded.root = old_root;
}

int Tree::CheckTree(const Node* node, const Node* lower, const Node* upper) const {
    if (node == nullptr) {
        return 0;
    }
    if (lower != nullptr && node->data <= lower->data) {
        return -1;
    }
    if (upper != nullptr && node->data >= upper->data) {
        return -1;
    }
    if (node->red) {
        if ((node->left != nullptr && node->left->red) ||
            (node->right != nullptr && node->right->red)) {
            return -1;
        }
    }
    int left = CheckTree(node->left, lower, node);
    int right = CheckTree(node->right, node, upper);
    if (left < 0 || right < 0 || left != right) {
        return -1;
    }
    return left + (node->red ? 0 : 1);
}
