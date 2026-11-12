#pragma once
#include "../spisok/spisok1.h"
#include "../serialization/serializable.h"
using namespace std;

class Queue1 : public Serializable {
private:
    FList list;
    ListNode* tail;

public:
    Queue1();
    Queue1(const Queue1& other);
    Queue1& operator=(const Queue1& other);

    void Q1InitQueue();
    void Q1PrintQueue() const;

    void Q1Insert(const string& chislo);
    void Q1Delete();

    string Q1Front() const;
    string Q1Back() const;

    void Q1PrintAddresses() const;
    ~Queue1() override;
    void Save(const string& filename) const override;
    void Load(const string& filename) override;
};
