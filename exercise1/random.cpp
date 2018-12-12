#include "random.h"

Random::Random(const string &key) : key(key.substr(0, 20))
{
    size_t seed = hash<string>{}(key);
    std::random_device rd;
    rng.seed(seed % INT_MAX);
}

int Random::next(int from, int to)
{
    std::uniform_int_distribution<int> distribution(from, to);
    return distribution(rng);
}

int Random::randomInt(int to)
{
    return randomInt(0, to);
}

int Random::randomInt(int from, int to)
{
    static std::mt19937 rng;
    static std::random_device rd;
    static bool seedSet = false;
    if (!seedSet) {
        rng.seed(rd());
        seedSet = true;
    }
    std::uniform_int_distribution<int> distribution(from, to);
    return distribution(rng);
}
