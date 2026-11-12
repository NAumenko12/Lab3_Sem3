#pragma once
#include <string>
using namespace std;

class ListNode {
public:
    string chislo;
    ListNode* next;

    explicit ListNode(const string& value, ListNode* next_node = nullptr)
        : chislo(value), next(next_node) {}
};
