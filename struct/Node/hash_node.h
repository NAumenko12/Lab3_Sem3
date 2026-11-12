#pragma once
#include <string>
using namespace std;

class HNode {
public:
    string key;
    string value;
    HNode* next;

    HNode(const string& key, const string& value, HNode* next_node = nullptr)
        : key(key), value(value), next(next_node) {}
};