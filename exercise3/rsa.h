#ifndef RSA_H
#define RSA_H

#include "bright.h"
#include "random.h"

class RSA
{
    Random engine;
    Bright p;
    Bright q;
    Bright e;

    Bright n;
    Bright fn;
    Bright d;
public:
    RSA(unsigned int keyLen);
    RSA(const Bright& p, const Bright& q, const Bright& e);

    Bright resetE();
    bool resetE(const Bright& e);

    Bright encrypt(const Bright& msg);
    Bright decrypt(const Bright& cipher);
};

#endif // RSA_H
