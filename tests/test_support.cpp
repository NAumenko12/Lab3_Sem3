#include "test_support.h"
#include <fstream>
#include <cstdio>
#include <stdexcept>
using namespace std;

Output::Output() : previous(cout.rdbuf(text.rdbuf())) {}
Output::~Output() {
    cout.rdbuf(previous);
}
string Output::Text() const {
    return text.str();
}
void Output::Clear() {
    text.str("");
    text.clear();
}

TestFile::TestFile() {
    const auto* test = testing::UnitTest::GetInstance()->current_test_info();
    name = string("test_") + test->test_suite_name() + "_" + test->name() + ".bin";
}
TestFile::~TestFile() {
    remove(name.c_str());
}
void TestFile::Write(const string& text) const {
    ofstream file(name, ios::binary);
    file.write(text.data(), text.size());
    if (!file)
        throw runtime_error("Не удалось подготовить тестовый файл");
}
string TestFile::Read() const {
    ifstream file(name, ios::binary);
    return string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
}
vector<string> Values(const FList& list) {
    vector<string> values;
    for (ListNode* node = list.GetHead(); node != nullptr; node = node->next) {
        values.push_back(node->chislo);
    }
    return values;
}
vector<string> Values(const LList& list) {
    vector<string> values;
    LNode* previous = nullptr;
    for (LNode* node = list.GetHead(); node != nullptr; node = node->next) {
        EXPECT_EQ(node->prev, previous);
        values.push_back(node->chislo);
        previous = node;
    }
    EXPECT_EQ(list.GetTail(), previous);
    return values;
}
