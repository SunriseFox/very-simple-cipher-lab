#ifndef CAESAR_H
#define CAESAR_H

#include <string>
#include <iostream>

#include "random.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define IsWin32
#include <Windows.h>
#endif

class Caesar
{
private:
    static char _shift(const char& c, int shift);

public:
    Caesar() = delete;

    static std::string encode(const std::string& plain, int shift);
    static std::string decode(const std::string& cipher, int shift);

    static std::string encodeRandom(const std::string& plain);
    static std::string decodeGuess(const std::string& cipher);
};

#endif // CAESAR_H
