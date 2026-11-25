#include "../serialization/file_archive.h"
#include <stdexcept>
#include "cuckoo_table.h"
#include <iostream>
#include <utility>
using namespace std;

CuckooCell::CuckooCell() : key(""), used(false) {}

CuckooTable::CuckooTable() : CuckooTable(make_unique<CuckooHash1>(), make_unique<CuckooHash2>()) {}

CuckooTable::CuckooTable(unique_ptr<HashFunction> firstHash, unique_ptr<HashFunction> secondHash)
    : firstHash(std::move(firstHash)), secondHash(std::move(secondHash)) {
    if (this->firstHash == nullptr) {
        this->firstHash = make_unique<CuckooHash1>();
    }
    if (this->secondHash == nullptr) {
        this->secondHash = make_unique<CuckooHash2>();
    }
}
CuckooTable::~CuckooTable() {}

int CuckooTable::CHash1(const string& key) const {
    return firstHash->HHash(key, 11);
}

int CuckooTable::CHash2(const string& key) const {
    return secondHash->HHash(key, 11);
}

void CuckooTable::CContains(const string& key, bool& found) const {
    int first_index = CHash1(key);
    int second_index = CHash2(key);
    found = (first[first_index].used && first[first_index].key == key) ||
            (second[second_index].used && second[second_index].key == key);
}

void CuckooTable::CInsert(const string& key, bool& inserted) {
    CContains(key, inserted);
    if (inserted) {
        return;
    }
    CuckooTable backup(*this);
    string current = key;
    for (int i = 0; i < 20; ++i) {
        int first_index = CHash1(current);
        if (!first[first_index].used) {
            first[first_index].key = current;
            first[first_index].used = true;
            inserted = true;
            return;
        }
        string displaced = first[first_index].key;
        first[first_index].key = current;
        current = displaced;
        int second_index = CHash2(current);
        if (!second[second_index].used) {
            second[second_index].key = current;
            second[second_index].used = true;
            inserted = true;
            return;
        }
        displaced = second[second_index].key;
        second[second_index].key = current;
        current = displaced;
    }
    *this = backup;
}

void CuckooTable::CDelete(const string& key) {
    int first_index = CHash1(key);
    int second_index = CHash2(key);
    if (first[first_index].used && first[first_index].key == key) {
        first[first_index].used = false;
    }
    if (second[second_index].used && second[second_index].key == key) {
        second[second_index].used = false;
    }
}

void CuckooTable::CPrint() const {
    cout << "Индекс | первая таблица | вторая таблица\n";
    for (int i = 0; i < 11; ++i) {
        const CuckooCell& first_cell = first[i];
        const CuckooCell& second_cell = second[i];
        cout << i << " | " << (first_cell.used ? first_cell.key : "-") << " | "
             << (second_cell.used ? second_cell.key : "-") << '\n';
    }
}
CuckooTable::CuckooTable(const CuckooTable& other)
    : firstHash(other.firstHash->Clone()), secondHash(other.secondHash->Clone()) {
    for (int i = 0; i < 11; ++i) {
        first[i] = other.first[i];
        second[i] = other.second[i];
    }
}

CuckooTable& CuckooTable::operator=(const CuckooTable& other) {
    if (this == &other) {
        return *this;
    }
    CuckooTable copy(other);
    swap(first, copy.first);
    swap(second, copy.second);
    swap(firstHash, copy.firstHash);
    swap(secondHash, copy.secondHash);
    return *this;
}

void CuckooTable::Save(const string& filename) const {
    FileWriter file(filename, "cuckoo");
    for (int table_index = 0; table_index < 2; ++table_index) {
        for (int i = 0; i < 11; ++i) {
            const CuckooCell& cell = table_index == 0 ? first[i] : second[i];
            file.WriteFlag(cell.used);
            if (!cell.used) {
                continue;
            }
            file.WriteString(cell.key);
        }
    }
    file.Finish();
}

void CuckooTable::Load(const string& filename) {
    FileReader file(filename, "cuckoo");
    CuckooTable loaded(firstHash->Clone(), secondHash->Clone());
    for (int table_index = 0; table_index < 2; ++table_index) {
        for (int i = 0; i < 11; ++i) {
            if (!file.ReadFlag()) {
                continue;
            }
            string key = file.ReadString();
            int index = table_index == 0 ? loaded.CHash1(key) : loaded.CHash2(key);
            if (index != i) {
                throw runtime_error("Ошибка: ключ не подходит выбранной хеш-функции");
            }
            bool found;
            loaded.CContains(key, found);
            if (found) {
                throw runtime_error("Ошибка: повторяющийся ключ");
            }
            CuckooCell& cell = table_index == 0 ? loaded.first[i] : loaded.second[i];
            cell.key = key;
            cell.used = true;
        }
    }
    file.Finish();
    swap(first, loaded.first);
    swap(second, loaded.second);
    swap(firstHash, loaded.firstHash);
    swap(secondHash, loaded.secondHash);
}
