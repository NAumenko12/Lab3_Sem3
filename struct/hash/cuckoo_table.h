#pragma once
#include <memory>
#include <string>
#include "../serialization/serializable.h"
#include "hash_function.h"
using namespace std;

class CuckooCell {
private:
    string key;
    bool used;

    friend class CuckooTable;

public:
    CuckooCell();
};

class CuckooTable : public Serializable {
private:
    CuckooCell first[11];
    CuckooCell second[11];

    unique_ptr<HashFunction> firstHash;
    unique_ptr<HashFunction> secondHash;

    int CHash1(const string& key) const;
    int CHash2(const string& key) const;

public:
    CuckooTable();
    CuckooTable(unique_ptr<HashFunction> firstHash, unique_ptr<HashFunction> secondHash);
    CuckooTable(const CuckooTable& other);
    CuckooTable& operator=(const CuckooTable& other);
    ~CuckooTable() override;
    void Save(const string& filename) const override;
    void Load(const string& filename) override;

    void CInsert(const string& key, bool& inserted);
    void CContains(const string& key, bool& found) const;
    void CDelete(const string& key);
    void CPrint() const;
};
