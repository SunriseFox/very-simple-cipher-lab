#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <iostream>
#include <bitset>

#include "bright.h"

class MathUtils;
#include "mathutils.h"

using namespace std;

class Random
{
    std::mt19937 rng;
public:
    Random();
    Random(const string& key);
    unsigned int nextInt(unsigned int from = 0, unsigned int to = 0xFFFFFFFF);
    Bright longInt(unsigned int length);
    Bright longPrime(unsigned int length);
};

#endif // RANDOM_H
