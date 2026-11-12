#include "hash_function.h"

HashFunction::~HashFunction() {}

int PolynomialHash::HHash(const string& key, int mesto) const {
    unsigned int hash = 0;
    for (unsigned char c : key) {
        hash = hash * 31 + c;
    }
    return hash % mesto;
}

unique_ptr<HashFunction> PolynomialHash::Clone() const {
    return make_unique<PolynomialHash>(*this);
}
int CuckooHash1::HHash(const string& key, int mesto) const {
    int hash = 0;
    for (unsigned char byte : key) {
        hash = (hash * 31 + byte) % mesto;
    }
    return hash;
}

unique_ptr<HashFunction> CuckooHash1::Clone() const {
    return make_unique<CuckooHash1>(*this);
}

int CuckooHash2::HHash(const string& key, int mesto) const {
    int hash = 0;
    for (unsigned char byte : key) {
        hash = (hash * 17 + byte + byte / mesto) % mesto;
    }
    return hash;
}

unique_ptr<HashFunction> CuckooHash2::Clone() const {
    return make_unique<CuckooHash2>(*this);
}