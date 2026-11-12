#pragma once
#include "../Node/tree_node.h"
#include <fstream>
#include "../serialization/serializable.h"

class Tree : public Serializable {
private:
    Node* root;
    bool IsRed(Node* node) const;
    void RotateLeft(Node*& node);
    void RotateRight(Node*& node);
    void Balance(Node*& node);
    void AddRecurs(Node*& node, int chislo);
    void BalanceDeleteLeft(Node*& node, bool& blackLost);
    void BalanceDeleteRight(Node*& node, bool& blackLost);
    void DeleteRecurs(Node*& node, int chislo, bool& blackLost);
    void Clear(Node*& node);
    Node* CopyNode(const Node* node);
    void SaveNode(ofstream& file, const Node* node) const;
    void LoadNode(ifstream& file, Node*& node);
    int CheckTree(const Node* node, const Node* lower, const Node* upper) const;

public:
    Tree();
    Tree(const Tree& other);
    Tree& operator=(const Tree& other);

    void InitTree();
    void Add(int chislo);
    void Delete(int chislo);
    void PrintPryam(Node* node) const;
    void PrintAddresses(Node* node) const;
    void Find(Node* node, int chislo) const;
    ~Tree() override;
    void Save(const string& filename) const override;
    void Load(const string& filename) override;

    Node* GetRoot() const;
};
