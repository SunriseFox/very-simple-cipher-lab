#ifndef MONOALPHABETIC_H
#define MONOALPHABETIC_H

#include <string>
#include <map>
#include <iostream>
#include <cctype>
#include <sstream>
#include <list>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define IsWin32
#include <Windows.h>
#endif

#include "random.h"

using namespace std;

struct ReplaceTable {
    map<int, int> transform;
    map<int, int> reverse;
};

class Monoalphabetic
{
private:
    static bool fillTable(ReplaceTable& table, const string& first, const string& second);
    static bool fillTable(ReplaceTable& table, int first, int second);

public:
    Monoalphabetic() = delete;
    static ReplaceTable genTable();
    static ReplaceTable genTable(const string& str);
    static string encode(const string& str, const map<int, int>& table);
    static string encodeWithTable(const string& str, const ReplaceTable& table);
    static string decodeWithTable(const string& str, const ReplaceTable& table);
    static ReplaceTable decodeGuess(string str);
};

#endif // MONOALPHABETIC_H
