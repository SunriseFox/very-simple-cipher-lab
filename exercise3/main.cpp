#include <iostream>

#include "bright.h"
#include "mathutils.h"
#include "rsa.h"

#include "random.h"
#include <ctime>

using namespace std;

int main()
{
    // RSA 算法和密钥长度（参数由 mt19937 随机数生成器 + 移位随机生成）
    RSA rsa(512);

    // 或者自己提供 p, q 和 e，会自动检验有效性和计算其他参数
    // Random rng;
    // RSA manual(rng.longPrime(512), rng.longPrime(512), e);

    // 需要加密的消息（十进制的 123456123456）
    Bright m = Bright("1CBE8EF240", 16);

    // 或者 10 进制等等
    // Bright m = Bright("123456123456）", 10);

    // 加密后的结果
    Bright c = rsa.encrypt(m);

    // 16 进制输出
    cout << hex << m << " is encrypted to " << c << endl << endl;

    // 或者 10 进制输出
    // cout << dec << m << " is encrypted to " << c << endl << endl;

    // 解密后的结果
    cout << hex << c << " is decrypted to " << rsa.decrypt(c) << endl;

    system("pause");
    return 0;
}
