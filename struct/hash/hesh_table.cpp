#include <stdexcept>
#include "hesh_table.h"
#include <utility>
#include <fstream>
#include <cstring>
#include <climits>
using namespace std;

HashTable::HashTable(int mesto) : HashTable(mesto, make_unique<PolynomialHash>()) {}

HashTable::HashTable(int mesto, unique_ptr<HashFunction> function)
    : data(nullptr), size(0), mesto(0), hashFunction(std::move(function)) {
    if (hashFunction == nullptr) {
        hashFunction = make_unique<PolynomialHash>();
    }
    HInitTable(mesto);
}

HashTable::~HashTable() {
    HDestroy();
}

void HashTable::HClear() {
    for (int i = 0; i < mesto; ++i) {
        HNode* node = data[i];
        while (node != nullptr) {
            HNode* next = node->next;
            delete node;
            node = next;
        }
        data[i] = nullptr;
    }
    size = 0;
}

void HashTable::HDestroy() {
    HClear();
    delete[] data;
    data = nullptr;
    size = 0;
    mesto = 0;
}

void HashTable::HInitTable(int capacity) {
    if (capacity < 1) {
        capacity = 1;
    }
    HNode** new_data = new HNode* [capacity] {};
    HDestroy();
    data = new_data;
    mesto = capacity;
}

int HashTable::HHash(const string& key, int capacity) const {
    return hashFunction->HHash(key, capacity);
}

void HashTable::HResize() {
    int new_mesto = mesto * 2;
    HNode** new_data = new HNode* [new_mesto] {};
    for (int i = 0; i < mesto; ++i) {
        HNode* node = data[i];
        while (node != nullptr) {
            HNode* next = node->next;
            int index = HHash(node->key, new_mesto);
            node->next = new_data[index];
            new_data[index] = node;
            node = next;
        }
    }
    delete[] data;
    data = new_data;
    mesto = new_mesto;
}

void HashTable::HInsert(const string& key, const string& value) {
    if (data == nullptr) {
        HInitTable(8);
    }
    int index = HHash(key, mesto);
    HNode* node = data[index];
    while (node != nullptr) {
        if (node->key == key) {
            node->value = value;
            return;
        }
        node = node->next;
    }
    data[index] = new HNode(key, value, data[index]);
    ++size;
    if (size >= mesto - mesto / 4) {
        HResize();
    }
}

void HashTable::HGet(const string& key, string& value, bool& found) const {
    found = false;
    if (data == nullptr) {
        return;
    }
    HNode* node = data[HHash(key, mesto)];
    while (node != nullptr) {
        if (node->key == key) {
            value = node->value;
            found = true;
            return;
        }
        node = node->next;
    }
}

void HashTable::HContains(const string& key, bool& found) const {
    string value;
    HGet(key, value, found);
}
void HashTable::HDelete(const string& key) {
    if (data == nullptr) {
        return;
    }
    int index = HHash(key, mesto);
    HNode* node = data[index];
    HNode* previous = nullptr;
    while (node != nullptr && node->key != key) {
        previous = node;
        node = node->next;
    }
    if (node == nullptr) {
        return;
    }
    if (previous == nullptr) {
        data[index] = node->next;
    } else {
        previous->next = node->next;
    }
    delete node;
    --size;
}

int HashTable::HLength() const {
    return size;
}

void HashTable::HPrint() const {
    for (int i = 0; i < mesto; ++i) {
        HNode* node = data[i];
        while (node != nullptr) {
            cout << node->key << " -> " << node->value << '\n';
            node = node->next;
        }
    }
}

void HashTable::HPrintAddresses() const {
    for (int i = 0; i < mesto; ++i) {
        cout << '[' << i << ']';
        HNode* node = data[i];
        while (node != nullptr) {
            cout << " -> " << node->key << ':' << node->value << " (" << node << ')';
            node = node->next;
        }
        cout << '\n';
    }
    cout << "size = " << size << " mesto = " << mesto << '\n';
}

HashTable::HashTable(const HashTable& other) : HashTable(other.mesto, other.hashFunction->Clone()) {
    if (other.data == nullptr) {
        HDestroy();
        return;
    }
    for (int i = 0; i < mesto; ++i) {
        HNode* node = other.data[i];
        HNode* previous = nullptr;
        while (node != nullptr) {
            HNode* new_node = new HNode(node->key, node->value);
            if (previous == nullptr) {
                data[i] = new_node;
            } else {
                previous->next = new_node;
            }
            previous = new_node;
            node = node->next;
        }
    }
    size = other.size;
}
HashTable& HashTable::operator=(const HashTable& other) {
    if (this == &other) {
        return *this;
    }
    HashTable copy(other);
    swap(data, copy.data);
    swap(size, copy.size);
    swap(mesto, copy.mesto);
    swap(hashFunction, copy.hashFunction);
    return *this;
}

void HashTable::Save(const string& filename) const {
    ofstream file(filename, ios::binary);
    if (!file) {
        throw runtime_error("Ошибка открытия файла");
    }
    size_t count = size;
    size_t capacity = mesto;
    char bytes[sizeof(size_t)];
    memcpy(bytes, &count, sizeof(count));
    file.write(bytes, sizeof(bytes));
    memcpy(bytes, &capacity, sizeof(capacity));
    file.write(bytes, sizeof(bytes));
    for (int i = 0; i < mesto; ++i) {
        HNode* node = data[i];
        while (node != nullptr) {
            size_t length = node->key.size();
            memcpy(bytes, &length, sizeof(length));
            file.write(bytes, sizeof(bytes));
            file.write(node->key.data(), length);
            length = node->value.size();
            memcpy(bytes, &length, sizeof(length));
            file.write(bytes, sizeof(bytes));
            file.write(node->value.data(), length);
            node = node->next;
        }
    }
    file.close();
    if (!file) {
        throw runtime_error("Ошибка записи файла");
    }
}

void HashTable::Load(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        throw runtime_error("Ошибка открытия файла");
    }
    char bytes[sizeof(size_t)];
    size_t count = 0;
    size_t capacity = 0;
    if (!file.read(bytes, sizeof(bytes))) {
        throw runtime_error("Ошибка чтения количества элементов");
    }
    memcpy(&count, bytes, sizeof(count));
    if (!file.read(bytes, sizeof(bytes))) {
        throw runtime_error("Ошибка чтения вместимости");
    }
    memcpy(&capacity, bytes, sizeof(capacity));
    if (count > capacity || capacity > INT_MAX) {
        throw runtime_error("Ошибка: неверный размер таблицы");
    }
    HashTable loaded(capacity, hashFunction->Clone());
    if (capacity == 0) {
        loaded.HDestroy();
    }
    for (size_t i = 0; i < count; ++i) {
        string entry[2];
        for (int j = 0; j < 2; ++j) {
            if (!file.read(bytes, sizeof(bytes))) {
                throw runtime_error("Ошибка чтения длины строки");
            }
            size_t length = 0;
            memcpy(&length, bytes, sizeof(length));
            for (size_t k = 0; k < length; ++k) {
                char c;
                if (!file.get(c)) {
                    throw runtime_error("Ошибка чтения строки");
                }
                entry[j] += c;
            }
        }
        loaded.HInsert(entry[0], entry[1]);
    }
    size_t actual = loaded.HLength();
    if (actual != count) {
        throw runtime_error("Ошибка: повторяющиеся ключи в файле");
    }
    if (file.peek() != char_traits<char>::eof() || file.bad()) {
        throw runtime_error("Ошибка: лишние данные или ошибка чтения");
    }
    swap(data, loaded.data);
    swap(size, loaded.size);
    swap(mesto, loaded.mesto);
    swap(hashFunction, loaded.hashFunction);
}
