#include "md5.h"

MD5::type_t MD5::a0 = 0x67452301;
MD5::type_t MD5::b0 = 0xefcdab89;
MD5::type_t MD5::c0 = 0x98badcfe;
MD5::type_t MD5::d0 = 0x10325476;
const unsigned char* MD5::buf[128];

MD5::type_t MD5::leftrotate(const type_t &x, const MD5::type_t &c)
{
    return (x << c) | (x >> (32-c));
}

void MD5::reintialize()
{
    a0 = 0x67452301;
    b0 = 0xefcdab89;
    c0 = 0x98badcfe;
    d0 = 0x10325476;
    memset(buf, 0, 128);
}

MD5::type_t MD5::nextBlockLen(const unsigned char *start)
{
    type_t i = 0;
    for (; i < 64; i++)
        if (start[i] == 0)
            break;
    return i;
}

MD5::return_t MD5::hash(const char * const &str, std::function<unsigned char*(const unsigned char*)> nextBlock, std::function<type_t(const unsigned char*)> blockLen)
{
    reintialize();

    ull length = 0;
    unsigned char* M = reinterpret_cast<unsigned char*>(const_cast<char *>(str));

    type_t l;
    bool shouldLoop = true;
    bool bitAppended = false;

    while (shouldLoop) {

        l = blockLen ? blockLen(M) : nextBlockLen(M);
        length += l;

        unsigned int * M_;

        if (l != 64) {
            memset(buf, 0, 64);
            memmove(buf, M, l);
            M = reinterpret_cast<unsigned char *>(buf);
            M_ = reinterpret_cast<unsigned int*>(M);
            if (!bitAppended) {
                M[l] = static_cast<unsigned char>(0x80);
                l++;
                bitAppended = true;
            }
            if (l > 56) {
                M += 64;
            } else {
                ull * t = reinterpret_cast<ull *>(M + 56);
                *t = length * 8;
                shouldLoop = false;
            }
        } else {
            M_ = reinterpret_cast<unsigned int*>(M);
        }

        type_t A = a0;
        type_t B = b0;
        type_t C = c0;
        type_t D = d0;

        for (type_t i = 0; i < 64; i++) {
            type_t F, g;
            if (i < 16) {
                F = (B & C) | ((~B) & D);
                g = i;
            } else if (i < 32) {
                F = (D & B) | ((~D) & C);
                g = (5 * i + 1) & 0x0F;
            } else if (i < 48) {
                F = B ^ C ^ D;
                g = (3 * i + 5) & 0x0F;
            } else {
                F = C ^ (B | (~D));
                g = (7 * i) & 0x0F;
            }
            F = F + A + K[i] + M_[g];
            A = D;
            D = C;
            C = B;
            B = B + leftrotate(F, s[i]);
        }

        a0 += A;
        b0 += B;
        c0 += C;
        d0 += D;

        if (l == 64) {
            if (nextBlock) {
                M = nextBlock(M);
            } else {
                M += l;
            }
        }
    }

    return_t ret;

    ret[0] = a0 << 24 >> 24;
    ret[1] = a0 << 16 >> 24;
    ret[2] = a0 << 8 >> 24;
    ret[3] = a0 << 0 >> 24;

    ret[4] = b0 << 24 >> 24;
    ret[5] = b0 << 16 >> 24;
    ret[6] = b0 << 8 >> 24;
    ret[7] = b0 << 0 >> 24;

    ret[8] = c0 << 24 >> 24;
    ret[9] = c0 << 16 >> 24;
    ret[10] = c0 <<8 >> 24;
    ret[11] = c0 <<0 >> 24;

    ret[12] = d0 << 24 >> 24;
    ret[13] = d0 << 16 >> 24;
    ret[14] = d0 << 8 >> 24;
    ret[15] = d0 << 0 >> 24;

    return ret;
}

MD5::return_t MD5::hashFile(const std::string &filename)
{
    ifstream fin(filename, ios_base::in | ios_base::binary);
    if (!fin.is_open()) {
        std::cerr << "failed to open file " << filename << endl;
        exit(EXIT_FAILURE);
    }
    char buf[65];
    volatile type_t blockLen;
    fin.read(buf, 64);
    blockLen = static_cast<unsigned>(fin.gcount());
    return hash(buf, [&](const unsigned char*){
        fin.read(buf, 64);
        blockLen = static_cast<unsigned>(fin.gcount());
        return reinterpret_cast<unsigned char*>(buf);
    }, [&](const unsigned char*){return blockLen;});
}
