#pragma once

class Node {
private:
    int data;
    bool red;
    Node* left;
    Node* right;
    friend class Tree;

public:
    Node(int chislo, bool color, Node* left_node = nullptr, Node* right_node = nullptr)
        : data(chislo), red(color), left(left_node), right(right_node) {}
};