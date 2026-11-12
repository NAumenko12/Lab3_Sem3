#include <chrono>
#include <iostream>
#include <string>
#include <stdexcept>
#include "struct/Array/massiv.h"
#include "struct/spisok/spisok1.h"
#include "struct/spisok/spisok2.h"
#include "struct/stack/stack.h"
#include "struct/ochered/ochered1.h"
#include "struct/tree/black_red_tree.h"
#include "struct/hash/hesh_table.h"
#include "struct/hash/cuckoo_table.h"
using namespace std;
using namespace std::chrono;

void PrintTime(const string& name, steady_clock::time_point start) {
    auto finish = steady_clock::now();
    cout << name << ": " << duration_cast<microseconds>(finish - start).count() << " мкс\n";
}

void BenchArray(int count) {
    auto start = steady_clock::now();
    Array array(count);
    for (int i = 0; i < count; ++i)
        array.MAddEnd("block");
    PrintTime("Массив, MAddEnd", start);
}

void BenchFList(int count) {
    auto start = steady_clock::now();
    FList list;
    for (int i = 0; i < count; ++i)
        list.FAddEnd("block");
    PrintTime("Односвязный список, FAddEnd", start);
}

void BenchLList(int count) {
    auto start = steady_clock::now();
    LList list;
    for (int i = 0; i < count; ++i)
        list.LAddEnd("block");
    PrintTime("Двусвязный список, LAddEnd", start);
}

void BenchStack(int count) {
    auto start = steady_clock::now();
    Stack stack;
    for (int i = 0; i < count; ++i)
        stack.SPush("block");
    PrintTime("Стек, SPush", start);
}

void BenchQueue(int count) {
    auto start = steady_clock::now();
    Queue1 queue;
    for (int i = 0; i < count; ++i)
        queue.Q1Insert("block");
    PrintTime("Очередь, Q1Insert", start);
}

void BenchHash(int count) {
    auto start = steady_clock::now();
    HashTable table(count);
    for (int i = 0; i < count; ++i)
        table.HInsert(to_string(i), "value");
    PrintTime("Цепочки, HInsert", start);
}

void BenchTree(int count) {
    auto start = steady_clock::now();
    Tree tree;
    for (int i = 0; i < count; ++i)
        tree.Add(i);
    PrintTime("Красно-чёрное дерево, Add", start);
}

void BenchCuckoo(int count) {
    auto start = steady_clock::now();
    for (int i = 0; i < count; ++i) {
        CuckooTable table;
        for (const string& key : {"a", "l", "w"}) {
            bool inserted;
            table.CInsert(key, inserted);
            if (!inserted)
                throw runtime_error("Ошибка вставки в бенчмарке кукушки");
        }
    }
    PrintTime("Кукушка, серии по 3 ключа с созданием и очисткой таблицы", start);
}

int main() {
    int count = 10000;
    cout << "Добавление " << count << " элементов; время включает создание структуры.\n";
    cout << "Для кукушки: " << count << " отдельных серий по 3 ключа.\n";
    try {
        BenchArray(count);
        BenchFList(count);
        BenchLList(count);
        BenchStack(count);
        BenchQueue(count);
        BenchHash(count);
        BenchTree(count);
        BenchCuckoo(count);
    } catch (const exception& error) {
        cerr << error.what() << '\n';
        return 1;
    }
}
