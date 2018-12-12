#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <iostream>

using namespace std;

class Random
{
    string key;
    std::mt19937 rng;

public:
    Random(const string& key);
    int next(int from, int to = 0);

    static int randomInt(int to = 0);
    static int randomInt(int from, int to = 0);
};

#endif // RANDOM_H
