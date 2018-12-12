#include "mathutils.h"

Bright MathUtils::gcd(const Bright &a, const Bright &b)
{
    if (b != 0) return gcd(b, a % b);
    return a;
}

void MathUtils::exgcd(Bright &a, Bright b, Bright &c, Bright e) {
    if (b == 0) throw c;
    exgcd(b, a % b, e, c - a / b * e);
}

Bright MathUtils::inv(const Bright &p, const Bright &q) {
    Bright a = p > q ? p : q;
    Bright b = p > q ? q : p;

    Bright c = p > q ? 0 : 1, e = p > q ? 1 : 0;
    try {
        exgcd(a, b, c, e);
    } catch (Bright& result) {
        result = result > 0 ? result : result += p;
        return result;
    }
    return 0;
}

bool MathUtils::probablePrime(const Bright &p) {
    for (int i = 0; i < 10000; i++)
        if (p % primes[i] == 0) {
            return p == primes[i];
        }
    return true;
}

bool MathUtils::millerPrime(const Bright &p, Random &random)
{
    if (!probablePrime(p)) 
        return false;
    Bright d = p - 1;
    while (d % 2 == 0)
    {
        d /= 2;
    }
    for (int i = 0; i < 5; i++)
    {
        Bright a = Bright(random.nextInt()) % (p - 1) + 1, temp = d;
        Bright mod = MathUtils::modulo(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1)
        {
            mod = mod * mod % p;
            temp *= 2;
        }
        if (mod != p - 1 && temp % 2 == 0)
            return false;
    }
    return true;
}

Bright MathUtils::modulo(const Bright &base, Bright exponent, const Bright &mod)
{
    Bright x = 1;
    Bright y = base;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            x = (x * y) % mod;
        y = (y * y) % mod;
        exponent = exponent / 2;
    }
    return x % mod;
}
