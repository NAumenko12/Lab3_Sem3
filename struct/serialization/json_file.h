#ifndef LAB3_JSON_FILE_H
#define LAB3_JSON_FILE_H

#include <iostream>
#include <string>
using namespace std;

void SkipJsonSpace(istream& file);
void ExpectJsonChar(istream& file, char expected);
string ReadJsonString(istream& file);
string ReadJsonNumber(istream& file);
void WriteJsonString(ostream& file, const string& value);

#endif
