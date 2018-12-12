#include <iostream>

#include "caesar.h"
#include "monoalphabetic.h"

using namespace std;

#define EXEC if (true)
#define SKIP if (false)

void debugTable(const ReplaceTable& table) {
    for (int i = 0; i < 26; i++) {
        cout << static_cast<char>('a' + i) << ' ';
    }
    cout << endl;
    for (int i = 0; i < 26; i++) {
        if (table.transform.count(i))
            cout << static_cast<char>('a' + table.transform.at(i)) << ' ';
        else if (table.transform.count('a' + i))
            cout << static_cast<char>(table.transform.at('a' + i)) << ' ';
        else
            cout << "_ ";
    }
    cout << endl;
}

void assert(const bool& ok) {
    (void)ok;
}

int main()
{
    int shift;
    string clearText, encodedText, decodedText;

    EXEC { // 1、自己创建明文信息，并选择一个密钥，实现移位密码加密和解密操作。

        cout << "Input <shift> <text> to encode: " << endl;

        cin >> shift;
        getline(cin, clearText);

        encodedText = Caesar::encode(clearText, shift);
        cout << endl << "encoded: " << endl << encodedText << endl;

        decodedText = Caesar::decode(encodedText, shift);
        cout << endl << "decoded: " << endl << decodedText << endl;

        assert(clearText == decodedText);

        cin.ignore(65535, '\n');
    }

    EXEC { // 2、随机移位加密，并通过猜测恢复密钥和明文

        cout << "Input <text> to encode: " << endl;

        getline(cin, clearText);

        encodedText = Caesar::encodeRandom(clearText);
        cout << endl << "encoded: " << endl;
        cout << encodedText << endl;

        cout << endl << "decode guess: " << endl;
        decodedText = Caesar::decodeGuess(encodedText);

        cout << endl << "decoded: " << endl;
        cout << decodedText << endl;

        assert(clearText == decodedText);

        cin.ignore(65535, '\n');
        cout << endl;

    }

    string key;
    ReplaceTable table;

    EXEC { // 3、通过任意字符串构建置换表，并实现置换密码的加解密

        cout << "Input a string as a table hash <text>: " << endl;

        getline(cin, key);

        table = Monoalphabetic::genTable(key);
        cout << endl << "Replace table: " << endl;
        debugTable(table);

        cout << endl << "Input <text> with previous table:  " << endl;

        getline(cin, clearText);

        cout << endl << "encoded: " << endl;
        encodedText = Monoalphabetic::encodeWithTable(clearText, table);
        cout << encodedText << endl;

        cout << endl << "decoded: " << endl;
        decodedText = Monoalphabetic::decodeWithTable(encodedText, table);
        cout << decodedText << endl;

        assert(clearText == decodedText);
    }

    EXEC { // 4、通过一定手段猜测置换表，并初步实现置换密码的加解密

        encodedText = "SIC GCBSPNA XPMHACQ JB GPYXSMEPNXIY JR SINS MF SPNBRQJSSJBE JBFMPQNSJMB FPMQ N XMJBS N SM N XMJBS H HY QCNBR MF N XMRRJHAY JBRCGZPC GINBBCA JB RZGI N VNY SINS SIC MPJEJBNA QCRRNEC GNB MBAY HC PCGMTCPCD HY SIC PJEISFZA PCGJXJCBSR SIC XNPSJGJXNBSR JB SIC SPNBRNGSJMB NPC NAJGC SIC MPJEJBNSMP MF SIC QCRRNEC HMH SIC PCGCJTCP NBD MRGNP N XMRRJHAC MXXMBCBS VIM VJRICR SM ENJB ZBNZSIMPJOCD GMBSPMA MF SIC QCRRNEC";

        cout << "original text: " << endl;

        Monoalphabetic::decodeGuess(encodedText);
    }

    return 0;
}
