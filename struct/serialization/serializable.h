#pragma once
#include <string>
using namespace std;

class Serializable {
public:
    virtual void Save(const string& filename) const = 0;
    virtual void Load(const string& filename) = 0;

    virtual ~Serializable();
};