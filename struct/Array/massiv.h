#pragma once
#include <iostream>
#include <string>
#include "../serialization/serializable.h"
using namespace std;

class Array : public Serializable {
private:
    string* data;
    int size;
    int mesto;

public:
    explicit Array(int mesto = 2);
    Array(const Array& other);
    Array& operator=(const Array& other);

    void MInitArray(int capacity);
    void MResize();

    void MAddEnd(const string& chislo);
    void MAddIndex(const string& chislo, int index);

    string MGet(int index) const;
    void MDeleteIndex(int index);
    void MReplace(const string& chislo, int index);

    int MLength() const;
    void MPrint() const;
    void MPrintAddresses() const;

    void MDestroy();
    ~Array() override;
    void Save(const string& filename) const override;
    void Load(const string& filename) override;
};
