#include "rsa.h"

RSA::RSA(unsigned int keyLen)
{
    p = engine.longPrime(keyLen);
    cout << hex << "p: " << p << endl << endl;

    q = engine.longPrime(keyLen);
    cout << hex << "q: " << q << endl << endl;

    n = p * q;
    cout << hex << "n (p * q): " << n << endl << endl;

    fn = (p-1) * (q-1);
    cout << hex << "fn [(p - 1) * (q - 1)]: " << fn << endl << endl;

    unsigned int i;
    while(true) {
        i = engine.nextInt(1000u, 9999u);
        if (fn > primes[i]
                && MathUtils::gcd(fn, primes[i]) == 1)
        {
            e = primes[i];
            break;
        }
    }
    cout << hex << "e [selected]: " << e << endl << endl;

    d = MathUtils::inv(fn, e);
    cout << hex << "d [inv(fn, e)]: " << d << endl << endl;
}

RSA::RSA(const Bright &p, const Bright &q, const Bright &e) : p(p), q(q)
{
    if (!resetE(e)) throw std::invalid_argument("e is not valid");
    d = MathUtils::inv(fn, e);
}

Bright RSA::resetE()
{
    unsigned int i;
    while(true)
    {
        i = engine.nextInt(1000u, 9999u);
        if (fn > primes[i]
                && MathUtils::gcd(fn, primes[i]) == 1)
        {
            e = primes[i];
            return e;
        }
    }
}

bool RSA::resetE(const Bright &_e)
{
    if (fn > _e && MathUtils::gcd(fn, _e) == 1)
    {
        this->e = _e;
        return true;
    }
    return false;
}

Bright RSA::encrypt(const Bright &msg)
{
    return MathUtils::modulo(msg, e, n);
}

Bright RSA::decrypt(const Bright &cipher)
{
    return MathUtils::modulo(cipher, d, n);
}
