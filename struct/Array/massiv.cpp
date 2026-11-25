#include "../serialization/file_archive.h"
#include <stdexcept>
#include "massiv.h"
#include <utility>
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
    FileWriter file(filename, "array");
    size_t count = size;
    size_t capacity = mesto;
    file.WriteSize(count);
    file.WriteSize(capacity);
    for (int i = 0; i < size; ++i) {
        file.WriteString(data[i]);
    }
    file.Finish();
}
void Array::Load(const string& filename) {
    FileReader file(filename, "array");
    size_t count = file.ReadSize();
    size_t capacity = file.ReadSize();
    if (count > capacity || capacity > INT_MAX) {
        throw runtime_error("Ошибка: неверный размер массива");
    }
    Array loaded(capacity);
    if (capacity == 0) {
        loaded.MDestroy();
    }
    for (size_t i = 0; i < count; ++i) {
        loaded.MAddEnd(file.ReadString());
    }
    file.Finish();
    swap(data, loaded.data);
    swap(size, loaded.size);
    swap(mesto, loaded.mesto);
}
