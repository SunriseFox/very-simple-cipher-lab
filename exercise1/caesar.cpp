#include "caesar.h"

char Caesar::_shift(const char &c, int shift)
{
    char r = c;
    if (!isalpha(c)) return r;
    if (shift < 0) shift += 26;
    bool isUpper = false;
    if ((r -= 'a') < 0) {
        isUpper = true;
        r += 'a' - 'A';
    }
    r += shift;
    r %= 26;
    r += isUpper ? 'A' : 'a';
    return r;
}

std::string Caesar::encode(const std::string &plain, int shift)
{
    std::string result;
    for (auto& c: plain) {
        result += _shift(c, -shift);
    }
    return result;
}

std::string Caesar::decode(const std::string &cipher, int shift)
{
    return encode(cipher, -shift);
}

std::string Caesar::encodeRandom(const std::string &plain)
{
    int rnd = Random::randomInt(0, 25);
    return encode(plain, rnd);
}

std::string Caesar::decodeGuess(const std::string &cipher)
{
#ifdef IsWin32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
    for (int i = 0; i < 26; i++) {
#ifdef IsWin32
        SetConsoleTextAttribute(hConsole, 7);
#endif
        std::cout << i << ": ";
#ifdef IsWin32
        SetConsoleTextAttribute(hConsole, i % 15 + 1);
#endif
        std::cout << decode(cipher.substr(0, 54) + (cipher.length() > 54 ? "..." : ""), i) << std::endl;
    }
#ifdef IsWin32
    SetConsoleTextAttribute(hConsole, 7);
#endif
    int i;
    std::cout << "which ? ";
    std::cin >> i;
    return decode(cipher, i);
}
