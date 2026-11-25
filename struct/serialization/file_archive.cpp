#include "file_archive.h"
#include "json_file.h"
#include <charconv>
#include <stdexcept>
using namespace std;

bool IsJsonFile(const string& filename) {
    return filename.size() >= 5 && filename.compare(filename.size() - 5, 5, ".json") == 0;
}

FileWriter::FileWriter(const string& filename, const string& type)
    : file(filename, ios::binary), json(IsJsonFile(filename)) {
    if (!file) {
        throw runtime_error("Ошибка открытия файла");
    }
    if (json) {
        file << "{\n  \"type\": ";
        WriteJsonString(file, type);
        file << ",\n  \"data\": [";
    }
}

void FileWriter::NextJsonValue() {
    if (!first) {
        file.put(',');
    }
    file << '\n' << "    ";
    first = false;
}

void FileWriter::WriteSize(size_t value) {
    if (json) {
        NextJsonValue();
        file << value;
    } else {
        file.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
}

void FileWriter::WriteInt(int value) {
    if (json) {
        NextJsonValue();
        file << value;
    } else {
        file.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
}

void FileWriter::WriteFlag(bool value) {
    if (json) {
        NextJsonValue();
    }
    file.put(value ? '1' : '0');
}

void FileWriter::WriteString(const string& value) {
    if (json) {
        NextJsonValue();
        WriteJsonString(file, value);
    } else {
        WriteSize(value.size());
        file.write(value.data(), value.size());
    }
}

void FileWriter::Finish() {
    if (json) {
        file << "\n  ]\n}\n";
    }
    file.close();
    if (!file) {
        throw runtime_error("Ошибка записи файла");
    }
}

FileReader::FileReader(const string& filename, const string& type)
    : file(filename, ios::binary), json(IsJsonFile(filename)) {
    if (!file) {
        throw runtime_error("Ошибка открытия файла");
    }
    if (json) {
        ExpectJsonChar(file, '{');
        if (ReadJsonString(file) != "type") {
            throw runtime_error("Ожидается поле type");
        }
        ExpectJsonChar(file, ':');
        if (ReadJsonString(file) != type) {
            throw runtime_error("Неверный тип структуры");
        }
        ExpectJsonChar(file, ',');
        if (ReadJsonString(file) != "data") {
            throw runtime_error("Ожидается поле data");
        }
        ExpectJsonChar(file, ':');
        ExpectJsonChar(file, '[');
    }
}

void FileReader::NextJsonValue() {
    if (!first) {
        ExpectJsonChar(file, ',');
    }
    first = false;
}

size_t FileReader::ReadSize() {
    size_t value;
    if (json) {
        NextJsonValue();
        string number = ReadJsonNumber(file);
        const char* end = number.data() + number.size();
        auto result = from_chars(number.data(), end, value);
        if (result.ec != errc{} || result.ptr != end) {
            throw runtime_error("Ошибка чтения размера");
        }
    } else if (!file.read(reinterpret_cast<char*>(&value), sizeof(value))) {
        throw runtime_error("Ошибка чтения размера");
    }
    return value;
}

int FileReader::ReadInt() {
    int value;
    if (json) {
        NextJsonValue();
        string number = ReadJsonNumber(file);
        const char* end = number.data() + number.size();
        auto result = from_chars(number.data(), end, value);
        if (result.ec != errc{} || result.ptr != end) {
            throw runtime_error("Ошибка чтения числа");
        }
    } else if (!file.read(reinterpret_cast<char*>(&value), sizeof(value))) {
        throw runtime_error("Ошибка чтения числа");
    }
    return value;
}

bool FileReader::ReadFlag() {
    if (json) {
        NextJsonValue();
        SkipJsonSpace(file);
    }
    char flag;
    if (!file.get(flag) || (flag != '0' && flag != '1')) {
        throw runtime_error("Ошибка чтения признака");
    }
    return flag == '1';
}

string FileReader::ReadString() {
    if (json) {
        NextJsonValue();
        return ReadJsonString(file);
    }
    size_t length = ReadSize();
    string value;
    for (size_t i = 0; i < length; ++i) {
        char c;
        if (!file.get(c)) {
            throw runtime_error("Ошибка чтения строки");
        }
        value += c;
    }
    return value;
}

void FileReader::Finish() {
    if (json) {
        ExpectJsonChar(file, ']');
        ExpectJsonChar(file, '}');
        SkipJsonSpace(file);
    }
    if (file.bad() || file.peek() != char_traits<char>::eof()) {
        throw runtime_error("Ошибка: лишние данные или ошибка чтения");
    }
}
