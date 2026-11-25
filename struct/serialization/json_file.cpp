#include "json_file.h"
#include <stdexcept>
using namespace std;

void SkipJsonSpace(istream& file) {
    while (true) {
        int c = file.peek();
        if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
            return;
        }
        file.get();
    }
}

void ExpectJsonChar(istream& file, char expected) {
    SkipJsonSpace(file);
    if (file.get() != expected) {
        throw runtime_error("Ошибка формата JSON");
    }
}

unsigned ReadHex(istream& file) {
    unsigned value = 0;
    for (int i = 0; i < 4; ++i) {
        int c = file.get();
        value *= 16;
        if (c >= '0' && c <= '9') {
            value += c - '0';
        } else if (c >= 'a' && c <= 'f') {
            value += c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            value += c - 'A' + 10;
        } else {
            throw runtime_error("Ошибка записи Unicode в JSON");
        }
    }
    return value;
}

void AppendUtf8(string& value, unsigned code) {
    if (code <= 0x7f) {
        value += char(code);
    } else if (code <= 0x7ff) {
        value += char(0xc0 | (code >> 6));
        value += char(0x80 | (code & 0x3f));
    } else if (code <= 0xffff) {
        value += char(0xe0 | (code >> 12));
        value += char(0x80 | ((code >> 6) & 0x3f));
        value += char(0x80 | (code & 0x3f));
    } else {
        value += char(0xf0 | (code >> 18));
        value += char(0x80 | ((code >> 12) & 0x3f));
        value += char(0x80 | ((code >> 6) & 0x3f));
        value += char(0x80 | (code & 0x3f));
    }
}

void ReadUnicode(istream& file, string& value) {
    unsigned code = ReadHex(file);
    if (code >= 0xd800 && code <= 0xdbff) {
        if (file.get() != '\\' || file.get() != 'u') {
            throw runtime_error("Ошибка записи Unicode в JSON");
        }
        unsigned low = ReadHex(file);
        if (low < 0xdc00 || low > 0xdfff) {
            throw runtime_error("Ошибка записи Unicode в JSON");
        }
        code = 0x10000 + ((code - 0xd800) << 10) + low - 0xdc00;
    } else if (code >= 0xdc00 && code <= 0xdfff) {
        throw runtime_error("Ошибка записи Unicode в JSON");
    }
    AppendUtf8(value, code);
}

string ReadJsonString(istream& file) {
    ExpectJsonChar(file, '"');
    string value;
    while (true) {
        int c = file.get();
        if (c == '"') {
            return value;
        }
        if (c < ' ') {
            throw runtime_error("Ошибка чтения строки JSON");
        }
        if (c != '\\') {
            value += char(c);
            continue;
        }
        switch (file.get()) {
        case '"':
            value += '"';
            break;
        case '\\':
            value += '\\';
            break;
        case '/':
            value += '/';
            break;
        case 'b':
            value += '\b';
            break;
        case 'f':
            value += '\f';
            break;
        case 'n':
            value += '\n';
            break;
        case 'r':
            value += '\r';
            break;
        case 't':
            value += '\t';
            break;
        case 'u':
            ReadUnicode(file, value);
            break;
        default:
            throw runtime_error("Ошибка экранирования строки JSON");
        }
    }
}

string ReadJsonNumber(istream& file) {
    SkipJsonSpace(file);
    string number;
    if (file.peek() == '-') {
        number += char(file.get());
    }
    if (file.peek() == '0') {
        number += char(file.get());
        return number;
    }
    while (file.peek() >= '0' && file.peek() <= '9') {
        number += char(file.get());
        if (number.size() > 21) {
            throw runtime_error("Слишком большое число в JSON");
        }
    }
    return number;
}

void WriteJsonString(ostream& file, const string& value) {
    const char hex[] = "0123456789abcdef";
    file.put('"');
    for (unsigned char c : value) {
        if (c == '"' || c == '\\') {
            file << '\\' << c;
        } else if (c < ' ') {
            file << "\\u00" << hex[c / 16] << hex[c % 16];
        } else {
            file << c;
        }
    }
    file.put('"');
}
