#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <bitset>

#include "md5.h"

using namespace std;

ostream& operator<< (ostream& os, const MD5::return_t& b) {
    os << hex;
    for (const auto& i : b) {
        os << std::setfill('0') << std::setw(sizeof(MD5::return_t::value_type) * 2) << static_cast<unsigned>(i);
    }
    return os;
}

string to_bits(const MD5::return_t& b) {
    stringstream os;
    for (const auto& i : b) {
        os << bitset<8>(i);
    }
    return os.str();
}

const struct Tests {
      const char *msg;
      unsigned char hash[16];
} tests[] = {
    { "",
      { 0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04,
        0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e }
    },
    { "a",
      {0x0c, 0xc1, 0x75, 0xb9, 0xc0, 0xf1, 0xb6, 0xa8,
       0x31, 0xc3, 0x99, 0xe2, 0x69, 0x77, 0x26, 0x61 }
    },
    { "abc",
      { 0x90, 0x01, 0x50, 0x98, 0x3c, 0xd2, 0x4f, 0xb0,
        0xd6, 0x96, 0x3f, 0x7d, 0x28, 0xe1, 0x7f, 0x72 }
    },
    { "message digest",
      { 0xf9, 0x6b, 0x69, 0x7d, 0x7c, 0xb7, 0x93, 0x8d,
        0x52, 0x5a, 0x2f, 0x31, 0xaa, 0xf1, 0x61, 0xd0 }
    },
    { "abcdefghijklmnopqrstuvwxyz",
      { 0xc3, 0xfc, 0xd3, 0xd7, 0x61, 0x92, 0xe4, 0x00,
        0x7d, 0xfb, 0x49, 0x6c, 0xca, 0x67, 0xe1, 0x3b }
    },
    { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
      { 0xd1, 0x74, 0xab, 0x98, 0xd2, 0x77, 0xd9, 0xf5,
        0xa5, 0x61, 0x1c, 0x2c, 0x9f, 0x41, 0x9d, 0x9f }
    },
    { "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
      { 0x57, 0xed, 0xf4, 0xa2, 0x2b, 0xe3, 0xc9, 0x55,
        0xac, 0x49, 0xda, 0x2e, 0x21, 0x07, 0xb6, 0x7a }
    },
};

int main(int argc, char* argv[])
{
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            cout << MD5::hashFile(argv[i]) << " " << argv[i] << endl;
        }
        return 0;
    }

    stringstream ssA;
    stringstream ssB;

    for (const auto &i : tests) {
        std::stringstream().swap(ssA);
        std::stringstream().swap(ssB);

        cout << "testing `" << i.msg << "`" << endl;
        MD5::return_t temp;
        std::copy(std::begin(i.hash), std::end(i.hash), std::begin(temp));

        ssA << temp;
        ssB << MD5::hash(i.msg);

        cout << "Expected: " << ssA.str() << endl;
        cout << "Got:      " << ssB.str() << endl;

        if (ssA.str() == ssB.str()) {
            cout << "+ same" << endl;
        } else {
            cout << "- wrong" << endl;
        }
        cout << endl;
    }

    // 检查改变的位

    const char* origin = "12345678901234567890";

    const char* test[] = {
        "112345678901234567890",
        "12345678901234567891",
        "12345678901234567890\n",
        "1234567890123456789",
        "1234567890123456789-",
        "123456789012345678901",
        "12345678911234567890",
        "12345678902134567890",
    };

    MD5::return_t ohash = MD5::hash(origin);
    string obits = to_bits(ohash);

    cout << "original string `" << origin << "`" << endl;
    cout <<"hash: \n  " << ohash << endl;
    cout << "bits: \n  " << obits << endl << endl;

    int totalDiff = 0;

    for (const auto& i: test) {
        MD5::return_t hash = MD5::hash(i);
        string bits = to_bits(hash);
        cout << "changed string `" << i << "`\nhash: \n  " << hash << endl;

        int diff = 0;
        for (int i = 0; i < obits.length(); i++) {
            if (obits.at(i) != bits.at(i))
                diff ++;
        }
        cout << "- bits changed: " << dec << diff << endl;
        cout << endl;

        totalDiff += diff;
    }

    int totalBits = obits.length() * sizeof(test)/sizeof (char*);

    cout << "average diff: " << totalDiff << "/" << totalBits << " (" << (double)totalDiff/totalBits << ")" << endl;

    return 0;
}
