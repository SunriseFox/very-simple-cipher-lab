#include "des.h"


void DES::_updateKey(const bit64 &key)
{
    // 私有方法：更新密钥，生成子密钥
    this->key = key;
    DEBUG(cout << "K: " << key << endl;)
    _generateSubkey();
}

void DES::_generateSubkey()
{
    // 私有方法：生成子密钥
    // 从 64 位密钥最终得到 16 个 48 位子密钥。

    // 1. 对输入密钥应用 PC-1<64, 56> 压缩置换表，生成 56 位密钥
    bit56 keyp = fillTable<64, 56>(key, TABLE_PC1);
    DEBUG(cout << "K+: " << keyp << endl;)

    // 2. 将 56 位分为前 28 位 C0 和 后 28 位 D0
    bit28 C[17], D[17];
    C[0] = bit28((keyp >> 28).to_ullong());
    D[0] = bit28((keyp << 28 >> 28).to_ullong());

    DEBUG(cout << "C0: " << C[0] << endl;)
    DEBUG(cout << "D0: " << D[0] << endl;)

    // 3. 对 Ci-1 和 Di-1 应用循环移位表，得到 Ci 和 Di
    for (int i = 1; i < 17; i++) {
        C[i] = shift<28>(C[i - 1], LSHIFT[i]);
        D[i] = shift<28>(D[i - 1], LSHIFT[i]);
        DEBUG(cout << "C" << i << ": " << C[i] << endl;)
        DEBUG(cout << "D" << i << ": " << D[i] << endl;)
    }

    // 4. 拼合 Ci 和 Di ，生成 Temp，
    // 5. 对 Temp 应用 PC-2<56, 48> 压缩置换表，生成 Ki
    for (size_t i = 1; i < 17; i++) {
        bit56 CD = bit56(C[i].to_ullong()) << 28 | bit56(D[i].to_ullong());
        K[i] = fillTable<56, 48>(CD, TABLE_PC2);
        DEBUG(cout << "K" << i << ": " << K[i] << endl;)
    }
}

bit64 DES::_encrypt(const bit64 &block, bool encrypt) const
{
    // 私有方法：加密或解密过程

    // 1. 对 TXT 应用 IP<64, 64> 置换表，得到 64 位 IP
    bit64 IP = fillTable<64, 64>(block, TABLE_IP);
    DEBUG(cout << "IP" << IP << endl;)

    bit32 L[17], R[17];

    // 2. 将 IP 分为 左 32 位 L0 和 右 32 位 R0
    L[0] = bit32((IP >> 32).to_ullong());
    R[0] = bit32((IP << 32 >> 32).to_ullong());

    DEBUG(cout << "L0: " << L[0] << endl;)
    DEBUG(cout << "R0: " << R[0] << endl;)



    // f 函数的定义
    auto f = [this](const bit32& R, const bit48& K) {

        // 1. 对 32 位 R 应用 E<32, 48> 扩展置换表，生成 Temp
        bit48 E = fillTable<32, 48>(R, TABLE_E);

        // 2. 令 E = E + K，其中 + 为异或运算
        E = E ^ K;

        DEBUG(cout << "E + K: " << E << endl;)

        // 3. 将 E 分为 6 位 8 组 B，
        //  对每组进行 S[i]<6, 4> 盒运算，生成 4 位 8 组 S
        //  拼合为 Temp
        bit6 B[9];
        for (size_t i = 1; i < 9; i++) {
            B[i] = (E << (6 * (i - 1)) >> 42).to_ullong();
            DEBUG(cout << "B" << i << ": " << B[i] << endl;)
        }

        bit4 S[9];
        bit32 s = 0;
        for (size_t i = 1; i < 9; i++) {
            S[i] = Si(i, B[i]);
            s |= bit32(S[i].to_ullong()) << ((8 - i) * 4);
            DEBUG(cout << "S" << i << ": " << S[i] << endl;)
        }

        // 4. 对 Temp 应用 P<32, 32> 置换表，生成 P
        bit32 P = fillTable<32, 32>(s, TABLE_P);
        DEBUG(cout << "P: " << P << endl;)
        return P;
    };

    // 3. 加密则令 Li = Ri-1 ，Ri = Li-1 + F(Ri-1, Ki)，解密则令Li = Ri-1，Ri = Li-1 + F(Ri-1, K17-i)
    for (int i = 1; i < 17; i++) {
        L[i] = R[i - 1];
        R[i] = L[i - 1] ^ f(R[i - 1], K[encrypt ? i : 17 - i]);
    }

    // 4. 拼合 R16, L16 为 64 位 RL
    bit64 RL = bit64(R[16].to_ullong() << 32 | L[16].to_ullong());

    // 5. 对 RL 应用 IP-1 <64, 64>得到最终结果

    bit64 res = fillTable<64, 64>(RL, TABLE_IP_REV);

    return res;
}

DES::DES(bit64 key)
{
    // 构造函数
    setKey(key);
}

bit4 DES::Si(size_t which, const bit6 &from) const
{
    // 从 S 盒中取得对应值
    unsigned int row
            = static_cast<unsigned int>(from[5]) << 1
            | static_cast<unsigned int>(from[0]) << 0;
    unsigned int col
            = static_cast<unsigned int>(from[4]) << 3
            | static_cast<unsigned int>(from[3]) << 2
            | static_cast<unsigned int>(from[2]) << 1
            | static_cast<unsigned int>(from[1]) << 0;
    bit4 res = S_BOX[which][row * 16 + col];
    return res;
}

void DES::setKey(const bit64 &key)
{
    // 公有方法：更新密钥
    _updateKey(key);
}

bit64 DES::encryptBlock(const bit64 &block) const
{
    // 公有方法：加密块
    return _encrypt(block, true);
}

bit64 DES::decryptBlock(const bit64 &block) const
{
    // 公有方法：解密块
    return _encrypt(block, false);
}


