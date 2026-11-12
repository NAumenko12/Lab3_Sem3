#pragma once
#include <memory>
#include <string>
using namespace std;

class HashFunction {
public:
    virtual int HHash(const string& key, int mesto) const = 0;
    virtual unique_ptr<HashFunction> Clone() const = 0;
    virtual ~HashFunction();
};

class PolynomialHash : public HashFunction {
public:
    int HHash(const string& key, int mesto) const override;
    unique_ptr<HashFunction> Clone() const override;
};

class CuckooHash1 : public HashFunction {
public:
    int HHash(const string& key, int mesto) const override;
    unique_ptr<HashFunction> Clone() const override;
};

class CuckooHash2 : public HashFunction {
public:
    int HHash(const string& key, int mesto) const override;
    unique_ptr<HashFunction> Clone() const override;
};