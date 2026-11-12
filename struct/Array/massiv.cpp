#include <stdexcept>
#include "massiv.h"
#include <utility>
#include <fstream>
#include <cstring>
#include <climits>
using namespace std;

Array::Array(int mesto) : data(nullptr), size(0), mesto(0) {
    MInitArray(mesto);
}

Array::~Array() {
    MDestroy();
}

void Array::MInitArray(int capacity) {
    if (capacity < 1) {
        capacity = 1;
    }
    string* new_data = new string[capacity];
    delete[] data;
    data = new_data;
    size = 0;
    mesto = capacity;
}

void Array::MDestroy() {
    delete[] data;
    data = nullptr;
    size = 0;
    mesto = 0;
}

void Array::MResize() {
    int new_mesto = 2;
    if (mesto > 0) {
        new_mesto = mesto * 2;
    }
    string* new_data = new string[new_mesto];
    for (int i = 0; i < size; ++i) {
        new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
    mesto = new_mesto;
}

void Array::MAddEnd(const string& chislo) {
    if (size == mesto) {
        MResize();
    }
    data[size] = chislo;
    ++size;
}

void Array::MAddIndex(const string& chislo, int index) {
    if (index < 0 || index > size) {
        cout << "no\n";
        return;
    }
    if (size == mesto) {
        MResize();
    }
    for (int i = size; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = chislo;
    ++size;
}
string Array::MGet(int index) const {
    if (index < 0 || index >= size) {
        cout << "no\n";
        return "";
    }
    return data[index];
}

void Array::MDeleteIndex(int index) {
    if (index < 0 || index >= size) {
        cout << "no\n";
        return;
    }
    for (int i = index; i < size - 1; ++i) {
        data[i] = data[i + 1];
    }
    --size;
}

void Array::MReplace(const string& chislo, int index) {
    if (index < 0 || index >= size) {
        cout << "no\n";
        return;
    }
    data[index] = chislo;
}

int Array::MLength() const {
    return size;
}

void Array::MPrint() const {
    for (int i = 0; i < size; ++i) {
        cout << data[i] << " ";
    }
    cout << "\n";
}

void Array::MPrintAddresses() const {
    for (int i = 0; i < size; ++i) {
        cout << "[" << i << "] " << data[i] << " | адрес: " << &data[i] << '\n';
    }
    cout << "size = " << size << " mesto = " << mesto << "\n\n";
}

Array::Array(const Array& other) : data(nullptr), size(0), mesto(0) {
    if (other.mesto == 0) {
        return;
    }
    Array copy(other.mesto);
    for (int i = 0; i < other.size; ++i) {
        copy.MAddEnd(other.data[i]);
    }
    data = copy.data;
    size = copy.size;
    mesto = copy.mesto;
    copy.data = nullptr;
    copy.size = 0;
    copy.mesto = 0;
}

Array& Array::operator=(const Array& other) {
    if (this == &other) {
        return *this;
    }
    Array copy(other);
    swap(data, copy.data);
    swap(size, copy.size);
    swap(mesto, copy.mesto);
    return *this;
}
void Array::Save(const string& filename) const {
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
    for (int i = 0; i < size; ++i) {
        size_t length = data[i].size();
        memcpy(bytes, &length, sizeof(length));
        file.write(bytes, sizeof(bytes));
        file.write(data[i].data(), length);
    }
    file.close();
    if (!file) {
        throw runtime_error("Ошибка записи файла");
    }
}
void Array::Load(const string& filename) {
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
        throw runtime_error("Ошибка: неверный размер массива");
    }
    Array loaded(capacity);
    if (capacity == 0) {
        loaded.MDestroy();
    }
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
        loaded.MAddEnd(value);
    }
    if (file.peek() != char_traits<char>::eof() || file.bad()) {
        throw runtime_error("Ошибка: лишние данные или ошибка чтения");
    }
    swap(data, loaded.data);
    swap(size, loaded.size);
    swap(mesto, loaded.mesto);
}
