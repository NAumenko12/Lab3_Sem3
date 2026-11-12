#pragma once
#include <string>
using namespace std;

class LNode {
public:
    string chislo;
    LNode* next;
    LNode* prev;

    explicit LNode(const string& value, LNode* next_node = nullptr, LNode* prev_node = nullptr)
        : chislo(value), next(next_node), prev(prev_node) {}
};