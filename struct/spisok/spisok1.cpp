#include <stdexcept>
#include "spisok1.h"
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

FList::FList() : head(nullptr) {}

void FList::FInitList() {
    while (head != nullptr) {
        FDeleteFirst();
    }
}

FList::~FList() {
    while (head != nullptr) {
        FDeleteFirst();
    }
}

void FList::FAddAfter(ListNode* flist, const string& chislo) {
    if (flist == nullptr) {
        cout << "no\n";
        return;
    }
    ListNode* new_node = new ListNode(chislo, flist->next);
    flist->next = new_node;
}

void FList::FAddFirst(const string& chislo) {
    ListNode* new_node = new ListNode(chislo, head);
    head = new_node;
}

void FList::FAddEnd(const string& chislo) {
    ListNode* new_node = new ListNode(chislo, nullptr);
    ListNode* cur_node = head;
    if (head == nullptr) {
        head = new_node;
        return;
    }
    while (cur_node->next != nullptr) {
        cur_node = cur_node->next;
    }
    cur_node->next = new_node;
}

void FList::FAddBefore(ListNode* target, const string& chislo) {
    if (target == nullptr) {
        cout << "no\n";
        return;
    }
    if (target == head) {
        FAddFirst(chislo);
        return;
    }
    ListNode* cur_node = head;
    while (cur_node != nullptr && cur_node->next != target) {
        cur_node = cur_node->next;
    }
    if (cur_node == nullptr) {
        cout << "no\n";
        return;
    }
    ListNode* new_node = new ListNode(chislo, target);
    cur_node->next = new_node;
}

void FList::FAddIndex(const string& chislo, int index) {
    if (index < 0) {
        cout << "no\n";
        return;
    }
    if (index == 0) {
        FAddFirst(chislo);
        return;
    }
    ListNode* cur_node = head;
    for (int i = 0; i < index - 1 && cur_node != nullptr; ++i) {
        cur_node = cur_node->next;
    }
    if (cur_node == nullptr) {
        cout << "no\n";
        return;
    }
    ListNode* new_node = new ListNode(chislo, cur_node->next);
    cur_node->next = new_node;
}

void FList::FDeleteAfter(ListNode* flist) {
    if (flist == nullptr || flist->next == nullptr) {
        cout << "no\n";
        return;
    }
    ListNode* delete_node = flist->next;
    flist->next = delete_node->next;
    delete delete_node;
}

void FList::FDeleteFirst() {
    if (head == nullptr) {
        cout << "no\n";
        return;
    }
    ListNode* delete_node = head;
    head = head->next;
    delete delete_node;
}

void FList::FDeleteEnd() {
    if (head == nullptr) {
        cout << "no\n";
        return;
    }
    if (head->next == nullptr) {
        FDeleteFirst();
        return;
    }
    ListNode* cur_node = head;
    while (cur_node->next->next != nullptr) {
        cur_node = cur_node->next;
    }
    FDeleteAfter(cur_node);
}

void FList::FDeleteBefore(ListNode* target) {
    if (head == nullptr || target == nullptr || target == head) {
        cout << "no\n";
        return;
    }
    if (head->next == target) {
        FDeleteFirst();
        return;
    }
    ListNode* cur_node = head;
    while (cur_node->next != nullptr && cur_node->next->next != target) {
        cur_node = cur_node->next;
    }
    if (cur_node->next == nullptr) {
        cout << "no\n";
        return;
    }
    FDeleteAfter(cur_node);
}

void FList::FDeleteIndex(int index) {
    if (index < 0) {
        cout << "no\n";
        return;
    }
    if (index == 0) {
        FDeleteFirst();
        return;
    }
    ListNode* cur_node = head;
    for (int i = 0; i < index - 1 && cur_node != nullptr; ++i) {
        cur_node = cur_node->next;
    }
    if (cur_node == nullptr || cur_node->next == nullptr) {
        cout << "no\n";
        return;
    }
    FDeleteAfter(cur_node);
}

void FList::FDeleteValue(const string& chislo) {
    if (head == nullptr) {
        return;
    }
    if (head->chislo == chislo) {
        FDeleteFirst();
        return;
    }
    ListNode* cur_node = head;
    while (cur_node->next != nullptr && cur_node->next->chislo != chislo) {
        cur_node = cur_node->next;
    }
    if (cur_node->next != nullptr) {
        FDeleteAfter(cur_node);
    }
}

void FList::FPrintBack(ListNode* flist) const {
    if (flist == nullptr) {
        return;
    }
    FPrintBack(flist->next);
    cout << flist->chislo << "\n";
}

void FList::FPrint(ListNode* flist) const {
    while (flist != nullptr) {
        cout << flist->chislo << "\n";
        flist = flist->next;
    }
}

void FList::FPrintAddresses(ListNode* flist) const {
    while (flist != nullptr) {
        cout << "Узел: " << flist << ", chislo: " << flist->chislo << ", next: " << flist->next
             << '\n';
        flist = flist->next;
    }
}

void FList::FFind(ListNode* flist, const string& chislo) const {
    while (flist != nullptr) {
        if (flist->chislo == chislo) {
            cout << chislo << " найден, адрес: " << flist << "\n";
            return;
        }
        flist = flist->next;
    }
    cout << chislo << " не найден\n";
}

ListNode* FList::GetHead() const {
    return head;
}

FList::FList(const FList& other) : head(nullptr) {
    FList copy;
    ListNode* cur_node = other.head;
    while (cur_node != nullptr) {
        copy.FAddEnd(cur_node->chislo);
        cur_node = cur_node->next;
    }
    head = copy.head;
    copy.head = nullptr;
}

FList& FList::operator=(const FList& other) {
    if (this == &other) {
        return *this;
    }
    FList copy(other);
    ListNode* old_head = head;
    head = copy.head;
    copy.head = old_head;
    return *this;
}

void FList::Save(const string& filename) const {
    ofstream file(filename, ios::binary);
    if (!file) {
        throw runtime_error("Ошибка открытия файла");
    }
    size_t count = 0;
    ListNode* node = head;
    while (node != nullptr) {
        ++count;
        node = node->next;
    }
    char bytes[sizeof(size_t)];
    memcpy(bytes, &count, sizeof(count));
    file.write(bytes, sizeof(bytes));
    node = head;
    while (node != nullptr) {
        size_t length = node->chislo.size();
        memcpy(bytes, &length, sizeof(length));
        file.write(bytes, sizeof(bytes));
        file.write(node->chislo.data(), length);
        node = node->next;
    }
    file.close();
    if (!file) {
        throw runtime_error("Ошибка записи файла");
    }
}

void FList::Load(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        throw runtime_error("Ошибка открытия файла");
    }
    char bytes[sizeof(size_t)];
    if (!file.read(bytes, sizeof(bytes))) {
        throw runtime_error("Ошибка чтения файла");
    }
    size_t count = 0;
    memcpy(&count, bytes, sizeof(count));
    FList loaded;
    for (size_t i = 0; i < count; ++i) {
        if (!file.read(bytes, sizeof(bytes))) {
            throw runtime_error("Ошибка чтения длины строки");
        }
        size_t length = 0;
        memcpy(&length, bytes, sizeof(length));
        string value;
        for (size_t j = 0; j < length; ++j) {
            char c;
            if (!file.get(c)) {
                throw runtime_error("Ошибка чтения строки");
            }
            value += c;
        }
        loaded.FAddEnd(value);
    }
    if (file.peek() != char_traits<char>::eof() || file.bad()) {
        throw runtime_error("Ошибка: лишние данные или ошибка чтения");
    }
    ListNode* old_head = head;
    head = loaded.head;
    loaded.head = old_head;
}
