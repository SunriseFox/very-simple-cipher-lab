#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "bright.h"
#include "knownprimes.h"

class Random;
#include "random.h"

class MathUtils
{
public:
    MathUtils() = delete;

    static Bright gcd(const Bright &a, const Bright& b);
    static void exgcd(Bright &a, Bright b, Bright &c, Bright e);

    static Bright inv(const Bright &p, const Bright &q);

    static bool probablePrime(const Bright &p);

    static bool millerPrime(const Bright& p, Random& random);

    static Bright modulo(const Bright &base, Bright exponent, const Bright &mod);
};

#endif // MATHUTILS_H
