#include "ochered1.h"
#include <iostream>
using namespace std;

Queue1::Queue1() : list(), tail(nullptr) {}

void Queue1::Q1InitQueue() {
    while (list.GetHead() != nullptr) {
        Q1Delete();
    }
    tail = nullptr;
}

void Queue1::Q1PrintQueue() const {
    list.FPrint(list.GetHead());
}

void Queue1::Q1Insert(const string& chislo) {
    if (tail == nullptr) {
        list.FAddFirst(chislo);
        tail = list.GetHead();
        return;
    }
    list.FAddAfter(tail, chislo);
    tail = tail->next;
}

void Queue1::Q1Delete() {
    list.FDeleteFirst();
    if (list.GetHead() == nullptr) {
        tail = nullptr;
    }
}

string Queue1::Q1Front() const {
    if (list.GetHead() == nullptr) {
        cout << "no\n";
        return "";
    }
    return list.GetHead()->chislo;
}

string Queue1::Q1Back() const {
    if (tail == nullptr) {
        cout << "no\n";
        return "";
    }
    return tail->chislo;
}

void Queue1::Q1PrintAddresses() const {
    list.FPrintAddresses(list.GetHead());
}

Queue1::Queue1(const Queue1& other) : list(other.list), tail(list.GetHead()) {
    if (tail != nullptr) {
        while (tail->next != nullptr) {
            tail = tail->next;
        }
    }
}

Queue1& Queue1::operator=(const Queue1& other) {
    if (this == &other) {
        return *this;
    }
    list = other.list;
    tail = list.GetHead();
    if (tail != nullptr) {
        while (tail->next != nullptr) {
            tail = tail->next;
        }
    }
    return *this;
}
Queue1::~Queue1() {}

void Queue1::Save(const string& filename) const {
    list.Save(filename);
}

void Queue1::Load(const string& filename) {
    list.Load(filename);
    tail = list.GetHead();
    if (tail != nullptr) {
        while (tail->next != nullptr) {
            tail = tail->next;
        }
    }
}
