#include "../serialization/file_archive.h"
#include <stdexcept>
#include "spisok2.h"
#include <iostream>
using namespace std;

LList::LList() : head(nullptr), tail(nullptr) {}

void LList::LInitList() {
    while (head != nullptr) {
        LDeleteFirst();
    }
    tail = nullptr;
}

LList::~LList() {
    while (head != nullptr) {
        LDeleteFirst();
    }
}

void LList::LPrint(LNode* flist) const {
    while (flist != nullptr) {
        cout << flist->chislo << "\n";
        flist = flist->next;
    }
}

void LList::LPrintBac(LNode* last) const {
    while (last != nullptr) {
        cout << last->chislo << '\n';
        last = last->prev;
    }
}

void LList::LAddAfter(LNode* flist, const string& chislo) {
    if (flist == nullptr) {
        cout << "no\n";
        return;
    }
    LNode* new_node = new LNode(chislo, flist->next, flist);
    if (flist->next != nullptr) {
        flist->next->prev = new_node;
    } else {
        tail = new_node;
    }
    flist->next = new_node;
}

void LList::LAddFirst(const string& chislo) {
    LNode* new_node = new LNode(chislo, head, nullptr);
    if (head != nullptr) {
        head->prev = new_node;
    } else {
        tail = new_node;
    }
    head = new_node;
}

void LList::LAddEnd(const string& chislo) {
    LNode* new_node = new LNode(chislo, nullptr, tail);
    if (tail != nullptr) {
        tail->next = new_node;
    } else {
        head = new_node;
    }
    tail = new_node;
}

void LList::LAddBefore(LNode* flist, const string& chislo) {
    if (flist == nullptr) {
        cout << "no\n";
        return;
    }
    LNode* new_node = new LNode(chislo, flist, flist->prev);
    if (flist->prev != nullptr) {
        flist->prev->next = new_node;
    } else {
        head = new_node;
    }
    flist->prev = new_node;
}

void LList::LAddIndex(const string& chislo, int index) {
    if (index < 0) {
        cout << "no\n";
        return;
    }
    if (index == 0) {
        LAddFirst(chislo);
        return;
    }
    LNode* cur_node = head;
    for (int i = 0; i < index - 1 && cur_node != nullptr; ++i) {
        cur_node = cur_node->next;
    }
    if (cur_node == nullptr) {
        cout << "no\n";
        return;
    }
    LAddAfter(cur_node, chislo);
}

void LList::LDeleteFirst() {
    if (head == nullptr) {
        cout << "no\n";
        return;
    }
    LNode* delete_node = head;
    head = head->next;
    if (head != nullptr) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
    }
    delete delete_node;
}

void LList::LDeleteEnd() {
    if (tail == nullptr) {
        cout << "no\n";
        return;
    }
    LNode* delete_node = tail;
    tail = tail->prev;
    if (tail != nullptr) {
        tail->next = nullptr;
    } else {
        head = nullptr;
    }
    delete delete_node;
}

void LList::LDeleteAfter(LNode* flist) {
    if (flist == nullptr || flist->next == nullptr) {
        cout << "no\n";
        return;
    }
    LNode* delete_node = flist->next;
    flist->next = delete_node->next;
    if (delete_node->next != nullptr) {
        delete_node->next->prev = flist;
    } else {
        tail = flist;
    }
    delete delete_node;
}

void LList::LDeleteBefore(LNode* flist) {
    if (flist == nullptr || flist->prev == nullptr) {
        cout << "no\n";
        return;
    }
    LNode* delete_node = flist->prev;
    flist->prev = delete_node->prev;
    if (delete_node->prev != nullptr) {
        delete_node->prev->next = flist;
    } else {
        head = flist;
    }
    delete delete_node;
}

void LList::LDeleteIndex(int index) {
    if (index < 0) {
        cout << "no\n";
        return;
    }
    LNode* cur_node = head;
    for (int i = 0; i < index && cur_node != nullptr; ++i) {
        cur_node = cur_node->next;
    }
    LDeleteNode(cur_node);
}

void LList::LDeleteNode(LNode* flist) {
    if (flist == nullptr) {
        cout << "no\n";
        return;
    }
    if (flist->prev != nullptr) {
        flist->prev->next = flist->next;
    } else {
        head = flist->next;
    }
    if (flist->next != nullptr) {
        flist->next->prev = flist->prev;
    } else {
        tail = flist->prev;
    }
    delete flist;
}

void LList::LDeleteValue(const string& chislo) {
    LNode* cur_node = head;
    while (cur_node != nullptr && cur_node->chislo != chislo) {
        cur_node = cur_node->next;
    }
    if (cur_node == nullptr) {
        return;
    }
    LDeleteNode(cur_node);
}

void LList::LFind(LNode* flist, const string& chislo) const {
    while (flist != nullptr) {
        if (flist->chislo == chislo) {
            cout << chislo << " найден, адрес: " << flist << "\n";
            return;
        }
        flist = flist->next;
    }
    cout << chislo << " не найден\n";
}

void LList::LPrintAddresses(LNode* first) const {
    LNode* cur_node = first;
    while (cur_node != nullptr) {
        cout << cur_node->chislo << " | адрес: " << cur_node << " | next: " << cur_node->next
             << " | prev: " << cur_node->prev << '\n';
        cur_node = cur_node->next;
    }
    cout << '\n';
}

LNode* LList::GetHead() const {
    return head;
}

LNode* LList::GetTail() const {
    return tail;
}
LList::LList(const LList& other) : head(nullptr), tail(nullptr) {
    LList copy;
    LNode* cur_node = other.head;

    while (cur_node != nullptr) {
        copy.LAddEnd(cur_node->chislo);
        cur_node = cur_node->next;
    }
    head = copy.head;
    tail = copy.tail;
    copy.head = nullptr;
    copy.tail = nullptr;
}

LList& LList::operator=(const LList& other) {
    if (this == &other) {
        return *this;
    }
    LList copy(other);
    LNode* old_head = head;
    LNode* old_tail = tail;
    head = copy.head;
    tail = copy.tail;
    copy.head = old_head;
    copy.tail = old_tail;
    return *this;
}

void LList::Save(const string& filename) const {
    FileWriter file(filename, "llist");
    size_t count = 0;
    LNode* node = head;
    while (node != nullptr) {
        ++count;
        node = node->next;
    }
    file.WriteSize(count);
    node = head;
    while (node != nullptr) {
        file.WriteString(node->chislo);
        node = node->next;
    }
    file.Finish();
}

void LList::Load(const string& filename) {
    FileReader file(filename, "llist");
    size_t count = file.ReadSize();
    LList loaded;
    for (size_t i = 0; i < count; ++i) {
        loaded.LAddEnd(file.ReadString());
    }
    file.Finish();
    LNode* old_head = head;
    LNode* old_tail = tail;
    head = loaded.head;
    tail = loaded.tail;
    loaded.head = old_head;
    loaded.tail = old_tail;
}
