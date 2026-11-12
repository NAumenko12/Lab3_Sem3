#pragma once
#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include <string>
#include "../struct/spisok/spisok1.h"
#include "../struct/spisok/spisok2.h"
using namespace std;

class Output {
    ostringstream text;
    streambuf* previous;

public:
    Output();
    ~Output();
    string Text() const;
    void Clear();
};

class TestFile {
public:
    string name;
    TestFile();
    ~TestFile();
    void Write(const string& text) const;
    string Read() const;
};

vector<string> Values(const FList& list);
vector<string> Values(const LList& list);
