#include "stack.h"
#include <iostream>
using namespace std;

Stack::Stack() : list() {}

void Stack::SInitStack() {
    while (list.GetHead() != nullptr) {
        list.FDeleteFirst();
    }
}

bool Stack::SIsEmptyStack() const {
    return list.GetHead() == nullptr;
}

void Stack::SPush(const string& chislo) {
    list.FAddFirst(chislo);
}

void Stack::SPop() {
    list.FDeleteFirst();
}

string Stack::STop() const {
    if (SIsEmptyStack()) {
        cout << "no\n";
        return "";
    }
    return list.GetHead()->chislo;
}

void Stack::SPrint(ListNode* flist) const {
    list.FPrint(flist);
}

void Stack::SPrintAddresses() const {
    list.FPrintAddresses(list.GetHead());
}

ListNode* Stack::GetHead() const {
    return list.GetHead();
}
Stack::~Stack() {}

void Stack::Save(const string& filename) const {
    list.Save(filename);
}

void Stack::Load(const string& filename) {
    list.Load(filename);
}
