#pragma once
#include "../spisok/spisok1.h"
#include "../serialization/serializable.h"
using namespace std;

class Stack : public Serializable {
private:
    FList list;

public:
    Stack();

    void SInitStack();
    bool SIsEmptyStack() const;

    void SPush(const string& chislo);
    void SPop();
    string STop() const;

    void SPrint(ListNode* flist) const;
    void SPrintAddresses() const;

    ListNode* GetHead() const;
    ~Stack() override;
    void Save(const string& filename) const override;
    void Load(const string& filename) override;
};
