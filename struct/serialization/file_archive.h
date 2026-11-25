#ifndef LAB3_FILE_ARCHIVE_H
#define LAB3_FILE_ARCHIVE_H

#include <fstream>
#include <string>
using namespace std;

class FileWriter {
private:
    ofstream file;
    bool json;
    bool first = true;
    void NextJsonValue();

public:
    FileWriter(const string& filename, const string& type);
    void WriteSize(size_t value);
    void WriteInt(int value);
    void WriteFlag(bool value);
    void WriteString(const string& value);
    void Finish();
};

class FileReader {
private:
    ifstream file;
    bool json;
    bool first = true;
    void NextJsonValue();

public:
    FileReader(const string& filename, const string& type);
    size_t ReadSize();
    int ReadInt();
    bool ReadFlag();
    string ReadString();
    void Finish();
};

#endif
