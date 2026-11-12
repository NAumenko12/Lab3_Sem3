#pragma once
#include <iostream>
#include <memory>
#include <string>
#include "../Node/hash_node.h"
#include "hash_function.h"
#include "../serialization/serializable.h"
using namespace std;

class HashTable : public Serializable {
private:
    HNode** data;
    int size;
    int mesto;
    unique_ptr<HashFunction> hashFunction;
    int HHash(const string& key, int capacity) const;
    void HResize();

public:
    explicit HashTable(int mesto = 8);
    HashTable(int mesto, unique_ptr<HashFunction> hashFunction);
    HashTable(const HashTable& other);
    HashTable& operator=(const HashTable& other);
    ~HashTable() override;
    void Save(const string& filename) const override;
    void Load(const string& filename) override;

    void HInitTable(int capacity);
    void HInsert(const string& key, const string& value);
    void HGet(const string& key, string& value, bool& found) const;
    void HContains(const string& key, bool& found) const;
    void HDelete(const string& key);
    int HLength() const;
    void HPrint() const;
    void HPrintAddresses() const;
    void HClear();
    void HDestroy();
};
