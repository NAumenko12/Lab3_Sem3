#include "ProcessCommand.h"
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
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

int RunFList(const string& filename, const string& command, istringstream& input);
int RunLList(const string& filename, const string& command, istringstream& input);
int RunStack(const string& filename, const string& command, istringstream& input);
int RunQueue(const string& filename, const string& command, istringstream& input);
int RunArray(const string& filename, const string& command, istringstream& input);
int RunTree(const string& filename, const string& command, istringstream& input);
int RunHash(const string& filename, const string& command, istringstream& input);
int RunCuckoo(const string& filename, const string& command, istringstream& input);

void PrintHelp() {
    cout << "Запуск:\n";
    cout << "./build/app --file файл.bin|файл.json --query 'команда данные'\n\n";
    cout << "Один файл хранит одну структуру. Сначала создайте его командой инициализации.\n";
    cout << "Повторная инициализация очищает выбранную структуру в файле.\n";
    cout << "Индексы начинаются с 0. Значения с пробелами заключайте в двойные кавычки.\n\n";
    cout << "Односвязный список:\n";
    cout << "FInitList | FAddFirst значение | FAddEnd значение\n";
    cout << "FAddIndex значение индекс | FAddBefore индекс значение | FAddAfter индекс значение\n";
    cout << "FDeleteFirst | FDeleteEnd | FDeleteIndex индекс | FDeleteValue значение\n";
    cout << "FDeleteBefore индекс | FDeleteAfter индекс\n";
    cout << "FPrint | FPrintBack | FPrintAddresses | FFind значение\n\n";
    cout << "Двусвязный список:\n";
    cout << "LInitList | LAddFirst значение | LAddEnd значение\n";
    cout << "LAddIndex значение индекс | LAddBefore индекс значение | LAddAfter индекс значение\n";
    cout << "LDeleteFirst | LDeleteEnd | LDeleteIndex индекс | LDeleteValue значение\n";
    cout << "LDeleteBefore индекс | LDeleteAfter индекс | LDeleteNode индекс\n";
    cout << "LPrint | LPrintBac | LPrintAddresses | LFind значение\n\n";
    cout << "Стек:\nSInitStack | SPush значение | SPop | STop\n";
    cout << "SIsEmptyStack | SPrint | SPrintAddresses\n\n";
    cout << "Очередь:\nQ1InitQueue | Q1Insert значение | Q1Delete\n";
    cout << "Q1Front | Q1Back | Q1PrintQueue | Q1PrintAddresses\n\n";
    cout << "Массив:\nMInitArray вместимость | MAddEnd значение | MAddIndex значение индекс\n";
    cout << "MGet индекс | MDeleteIndex индекс | MReplace значение индекс\n";
    cout << "MLength | MPrint | MPrintAddresses | MResize | MDestroy\n\n";
    cout << "Дерево:\nTInitTree | TAdd число | TDelete число | TFind число\n";
    cout << "TPrint | TPrintPryam | TPrintAddresses\n\n";
    cout << "Хеш-таблица с цепочками:\nHInitTable вместимость | HInsert ключ значение\n";
    cout << "HGet ключ | HContains ключ | HDelete ключ\n";
    cout << "HLength | HPrint | HPrintAddresses | HClear | HDestroy\n\n";
    cout << "Кукушка:\nCInitTable | CInsert ключ | CContains ключ | CDelete ключ | CPrint\n";
}

string ReadValue(istringstream& input) {
    string value;
    if (!(input >> quoted(value))) {
        throw invalid_argument("не хватает аргументов команды");
    }
    return value;
}

int ReadNumber(istringstream& input) {
    string value = ReadValue(input);
    istringstream number(value);
    int result;
    char extra;
    if (!(number >> result) || number >> extra) {
        throw invalid_argument("ожидалось целое число типа int");
    }
    return result;
}

void CheckEnd(istringstream& input) {
    string extra;
    if (input >> extra) {
        throw invalid_argument("лишние аргументы команды");
    }
}

ListNode* FindFNode(ListNode* node, int index) {
    if (index < 0) {
        throw out_of_range("индекс не может быть отрицательным");
    }
    for (int i = 0; i < index && node != nullptr; ++i) {
        node = node->next;
    }
    if (node == nullptr) {
        throw out_of_range("нет узла с таким индексом");
    }
    return node;
}

LNode* FindLNode(LNode* node, int index) {
    if (index < 0) {
        throw out_of_range("индекс не может быть отрицательным");
    }
    for (int i = 0; i < index && node != nullptr; ++i) {
        node = node->next;
    }
    if (node == nullptr) {
        throw out_of_range("нет узла с таким индексом");
    }
    return node;
}

int ProcessCommand(const string& filename, const string& query) {
    istringstream input(query);
    string command;
    if (!(input >> command)) {
        throw invalid_argument("пустая команда");
    }
    if (command[0] == 'F')
        return RunFList(filename, command, input);
    if (command[0] == 'L')
        return RunLList(filename, command, input);
    if (command[0] == 'S')
        return RunStack(filename, command, input);
    if (command.substr(0, 2) == "Q1")
        return RunQueue(filename, command, input);
    if (command[0] == 'M')
        return RunArray(filename, command, input);
    if (command[0] == 'T')
        return RunTree(filename, command, input);
    if (command[0] == 'H')
        return RunHash(filename, command, input);
    if (command[0] == 'C')
        return RunCuckoo(filename, command, input);
    throw invalid_argument("неизвестная команда: " + command);
}

int RunFList(const string& filename, const string& command, istringstream& input) {
    FList list;
    if (command == "FInitList") {
        CheckEnd(input);
        list.FInitList();
        list.Save(filename);
        return 0;
    }
    list.Load(filename);
    if (command == "FPrint" || command == "FPrintBack" || command == "FPrintAddresses") {
        CheckEnd(input);
        if (command == "FPrint")
            list.FPrint(list.GetHead());
        else if (command == "FPrintBack")
            list.FPrintBack(list.GetHead());
        else
            list.FPrintAddresses(list.GetHead());
        return 0;
    }
    if (command == "FFind") {
        string value = ReadValue(input);
        CheckEnd(input);
        list.FFind(list.GetHead(), value);
        return 0;
    }
    if (command == "FAddFirst" || command == "FAddEnd" || command == "FDeleteValue") {
        string value = ReadValue(input);
        CheckEnd(input);
        if (command == "FAddFirst")
            list.FAddFirst(value);
        else if (command == "FAddEnd")
            list.FAddEnd(value);
        else
            list.FDeleteValue(value);
    } else if (command == "FAddIndex") {
        string value = ReadValue(input);
        int index = ReadNumber(input);
        CheckEnd(input);
        if (index < 0)
            throw out_of_range("отрицательный индекс");
        if (index > 0)
            FindFNode(list.GetHead(), index - 1);
        list.FAddIndex(value, index);
    } else if (command == "FAddBefore" || command == "FAddAfter") {
        int index = ReadNumber(input);
        string value = ReadValue(input);
        CheckEnd(input);
        ListNode* node = FindFNode(list.GetHead(), index);
        if (command == "FAddBefore")
            list.FAddBefore(node, value);
        else
            list.FAddAfter(node, value);
    } else if (command == "FDeleteFirst" || command == "FDeleteEnd") {
        CheckEnd(input);
        FindFNode(list.GetHead(), 0);
        if (command == "FDeleteFirst")
            list.FDeleteFirst();
        else
            list.FDeleteEnd();
    } else if (command == "FDeleteIndex" || command == "FDeleteBefore" ||
               command == "FDeleteAfter") {
        int index = ReadNumber(input);
        CheckEnd(input);
        ListNode* node = FindFNode(list.GetHead(), index);
        if (command == "FDeleteBefore") {
            if (index == 0)
                throw out_of_range("перед первым узлом нет элемента");
            list.FDeleteBefore(node);
        } else if (command == "FDeleteAfter") {
            if (node->next == nullptr)
                throw out_of_range("после последнего узла нет элемента");
            list.FDeleteAfter(node);
        } else
            list.FDeleteIndex(index);
    } else {
        throw invalid_argument("неизвестная команда списка: " + command);
    }
    list.Save(filename);
    return 0;
}

int RunLList(const string& filename, const string& command, istringstream& input) {
    LList list;
    if (command == "LInitList") {
        CheckEnd(input);
        list.LInitList();
        list.Save(filename);
        return 0;
    }
    list.Load(filename);
    if (command == "LPrint" || command == "LPrintBac" || command == "LPrintAddresses") {
        CheckEnd(input);
        if (command == "LPrint")
            list.LPrint(list.GetHead());
        else if (command == "LPrintBac")
            list.LPrintBac(list.GetTail());
        else
            list.LPrintAddresses(list.GetHead());
        return 0;
    }
    if (command == "LFind") {
        string value = ReadValue(input);
        CheckEnd(input);
        list.LFind(list.GetHead(), value);
        return 0;
    }
    if (command == "LAddFirst" || command == "LAddEnd" || command == "LDeleteValue") {
        string value = ReadValue(input);
        CheckEnd(input);
        if (command == "LAddFirst")
            list.LAddFirst(value);
        else if (command == "LAddEnd")
            list.LAddEnd(value);
        else
            list.LDeleteValue(value);
    } else if (command == "LAddIndex") {
        string value = ReadValue(input);
        int index = ReadNumber(input);
        CheckEnd(input);
        if (index < 0)
            throw out_of_range("отрицательный индекс");
        if (index > 0)
            FindLNode(list.GetHead(), index - 1);
        list.LAddIndex(value, index);
    } else if (command == "LAddBefore" || command == "LAddAfter") {
        int index = ReadNumber(input);
        string value = ReadValue(input);
        CheckEnd(input);
        LNode* node = FindLNode(list.GetHead(), index);
        if (command == "LAddBefore")
            list.LAddBefore(node, value);
        else
            list.LAddAfter(node, value);
    } else if (command == "LDeleteFirst" || command == "LDeleteEnd") {
        CheckEnd(input);
        FindLNode(list.GetHead(), 0);
        if (command == "LDeleteFirst")
            list.LDeleteFirst();
        else
            list.LDeleteEnd();
    } else if (command == "LDeleteIndex" || command == "LDeleteBefore" ||
               command == "LDeleteAfter" || command == "LDeleteNode") {
        int index = ReadNumber(input);
        CheckEnd(input);
        LNode* node = FindLNode(list.GetHead(), index);
        if (command == "LDeleteBefore") {
            if (node->prev == nullptr)
                throw out_of_range("перед первым узлом нет элемента");
            list.LDeleteBefore(node);
        } else if (command == "LDeleteAfter") {
            if (node->next == nullptr)
                throw out_of_range("после последнего узла нет элемента");
            list.LDeleteAfter(node);
        } else if (command == "LDeleteNode")
            list.LDeleteNode(node);
        else
            list.LDeleteIndex(index);
    } else {
        throw invalid_argument("неизвестная команда списка: " + command);
    }
    list.Save(filename);
    return 0;
}

int RunStack(const string& filename, const string& command, istringstream& input) {
    Stack stack;
    if (command == "SInitStack") {
        CheckEnd(input);
        stack.SInitStack();
        stack.Save(filename);
        return 0;
    }
    stack.Load(filename);
    if (command == "SPush") {
        string value = ReadValue(input);
        CheckEnd(input);
        stack.SPush(value);
    } else if (command == "SPop") {
        CheckEnd(input);
        if (stack.SIsEmptyStack())
            throw out_of_range("стек пуст");
        stack.SPop();
    } else {
        CheckEnd(input);
        if (command == "STop") {
            if (stack.SIsEmptyStack())
                throw out_of_range("стек пуст");
            cout << stack.STop() << '\n';
        } else if (command == "SIsEmptyStack")
            cout << boolalpha << stack.SIsEmptyStack() << '\n';
        else if (command == "SPrint")
            stack.SPrint(stack.GetHead());
        else if (command == "SPrintAddresses")
            stack.SPrintAddresses();
        else
            throw invalid_argument("неизвестная команда стека: " + command);
        return 0;
    }
    stack.Save(filename);
    return 0;
}

int RunQueue(const string& filename, const string& command, istringstream& input) {
    Queue1 queue;
    if (command == "Q1InitQueue") {
        CheckEnd(input);
        queue.Q1InitQueue();
        queue.Save(filename);
        return 0;
    }
    queue.Load(filename);
    if (command == "Q1Insert") {
        string value = ReadValue(input);
        CheckEnd(input);
        queue.Q1Insert(value);
    } else if (command == "Q1Delete") {
        CheckEnd(input);
        queue.Q1Delete();
    } else {
        CheckEnd(input);
        if (command == "Q1Front")
            cout << queue.Q1Front() << '\n';
        else if (command == "Q1Back")
            cout << queue.Q1Back() << '\n';
        else if (command == "Q1PrintQueue")
            queue.Q1PrintQueue();
        else if (command == "Q1PrintAddresses")
            queue.Q1PrintAddresses();
        else
            throw invalid_argument("неизвестная команда очереди: " + command);
        return 0;
    }
    queue.Save(filename);
    return 0;
}

int RunArray(const string& filename, const string& command, istringstream& input) {
    Array array;
    if (command == "MInitArray") {
        int capacity = ReadNumber(input);
        CheckEnd(input);
        if (capacity < 1)
            throw invalid_argument("вместимость должна быть положительной");
        array.MInitArray(capacity);
        array.Save(filename);
        return 0;
    }
    array.Load(filename);
    if (command == "MAddEnd") {
        string value = ReadValue(input);
        CheckEnd(input);
        array.MAddEnd(value);
    } else if (command == "MAddIndex" || command == "MReplace") {
        string value = ReadValue(input);
        int index = ReadNumber(input);
        CheckEnd(input);
        int limit = array.MLength();
        if (command == "MReplace")
            --limit;
        if (index < 0 || index > limit)
            throw out_of_range("индекс за границами массива");
        if (command == "MAddIndex")
            array.MAddIndex(value, index);
        else
            array.MReplace(value, index);
    } else if (command == "MDeleteIndex" || command == "MGet") {
        int index = ReadNumber(input);
        CheckEnd(input);
        if (index < 0 || index >= array.MLength())
            throw out_of_range("индекс за границами массива");
        if (command == "MGet") {
            cout << array.MGet(index) << '\n';
            return 0;
        }
        array.MDeleteIndex(index);
    } else {
        CheckEnd(input);
        if (command == "MDestroy")
            array.MDestroy();
        else if (command == "MResize")
            array.MResize();
        else {
            if (command == "MLength")
                cout << array.MLength() << '\n';
            else if (command == "MPrint")
                array.MPrint();
            else if (command == "MPrintAddresses")
                array.MPrintAddresses();
            else
                throw invalid_argument("неизвестная команда массива: " + command);
            return 0;
        }
    }
    array.Save(filename);
    return 0;
}

int RunTree(const string& filename, const string& command, istringstream& input) {
    Tree tree;
    if (command == "TInitTree") {
        CheckEnd(input);
        tree.InitTree();
        tree.Save(filename);
        return 0;
    }
    tree.Load(filename);
    if (command == "TAdd" || command == "TDelete" || command == "TFind") {
        int value = ReadNumber(input);
        CheckEnd(input);
        if (command == "TFind") {
            tree.Find(tree.GetRoot(), value);
            return 0;
        }
        if (command == "TAdd")
            tree.Add(value);
        else
            tree.Delete(value);
    } else {
        CheckEnd(input);
        if (command == "TPrint" || command == "TPrintPryam") {
            tree.PrintPryam(tree.GetRoot());
            cout << '\n';
        } else if (command == "TPrintAddresses")
            tree.PrintAddresses(tree.GetRoot());
        else
            throw invalid_argument("неизвестная команда дерева: " + command);
        return 0;
    }
    tree.Save(filename);
    return 0;
}

int RunHash(const string& filename, const string& command, istringstream& input) {
    HashTable table;
    if (command == "HInitTable") {
        int capacity = ReadNumber(input);
        CheckEnd(input);
        if (capacity < 1)
            throw invalid_argument("вместимость должна быть положительной");
        table.HInitTable(capacity);
        table.Save(filename);
        return 0;
    }
    table.Load(filename);
    if (command == "HInsert") {
        string key = ReadValue(input);
        string value = ReadValue(input);
        CheckEnd(input);
        table.HInsert(key, value);
    } else if (command == "HGet" || command == "HContains" || command == "HDelete") {
        string key = ReadValue(input);
        CheckEnd(input);
        if (command == "HDelete")
            table.HDelete(key);
        else {
            bool found;
            if (command == "HContains") {
                table.HContains(key, found);
                cout << boolalpha << found << '\n';
            } else {
                string value;
                table.HGet(key, value, found);
                if (found)
                    cout << value << '\n';
                else
                    cout << "Ключ не найден\n";
            }
            return 0;
        }
    } else {
        CheckEnd(input);
        if (command == "HClear")
            table.HClear();
        else if (command == "HDestroy")
            table.HDestroy();
        else {
            if (command == "HLength")
                cout << table.HLength() << '\n';
            else if (command == "HPrint")
                table.HPrint();
            else if (command == "HPrintAddresses")
                table.HPrintAddresses();
            else
                throw invalid_argument("неизвестная команда хеш-таблицы: " + command);
            return 0;
        }
    }
    table.Save(filename);
    return 0;
}

int RunCuckoo(const string& filename, const string& command, istringstream& input) {
    CuckooTable table;
    if (command == "CInitTable") {
        CheckEnd(input);
        table.Save(filename);
        return 0;
    }
    table.Load(filename);
    if (command == "CPrint") {
        CheckEnd(input);
        table.CPrint();
        return 0;
    }
    if (command != "CInsert" && command != "CContains" && command != "CDelete") {
        throw invalid_argument("неизвестная команда кукушки: " + command);
    }
    string key = ReadValue(input);
    CheckEnd(input);
    if (command == "CContains") {
        bool found;
        table.CContains(key, found);
        cout << boolalpha << found << '\n';
        return 0;
    }
    if (command == "CInsert") {
        bool inserted;
        table.CInsert(key, inserted);
        if (!inserted) {
            throw runtime_error("не удалось вставить ключ: цикл вытеснений или нет места");
        }
    } else
        table.CDelete(key);
    table.Save(filename);
    return 0;
}
