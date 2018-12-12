#include "random.h"

Random::Random()
{
    static std::random_device rd;
    rng.seed(rd());
}

Random::Random(const string &key)
{
    size_t seed = hash<string>{}(key);
    rng.seed(seed % UINT_MAX);
}

unsigned int Random::nextInt(unsigned int from, unsigned int to)
{
    std::uniform_int_distribution<unsigned int> distribution(from, to);
    return distribution(rng);
}

Bright Random::longInt(unsigned int length)
{
    Bright res;
    size_t m = length % Bright::base2;
    size_t s = length / Bright::base2 + (m != 0);

    if (m == 0) m = Bright::base2 - 1;
    else --m;
    res.unshift(nextInt(0, 1u << m));
    for (size_t i = 0; i < s - 1; i++) {
        res.unshift(nextInt(0, Bright::mask));
    }
    return res;
}

Bright Random::longPrime(unsigned int length)
{
    auto f = [&]{
        Bright res;
        size_t m = length % Bright::base2;
        size_t s = length / Bright::base2 + (m != 0);

        if (m == 0) m = Bright::base2 - 1;
        else --m;
        res.unshift(nextInt(0, 1u << m) | 1u << m);
        for (size_t i = 0; i < s - 2; i++) {
            res.unshift(nextInt(0, Bright::mask));
        }
        if (s > 1)
            res.unshift(nextInt(0, Bright::mask) | 1);
        return res;
    };
    Bright p;
    while (true) {
        p = f();
        if (MathUtils::millerPrime(p, *this))
            return p;
    }
}

