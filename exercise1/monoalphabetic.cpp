#include "monoalphabetic.h"

#include <cassert>

ReplaceTable tryFillTable(const string& str, ReplaceTable& table) {
    while (table.transform.size() != 26) {
        cout << endl << "current information: " << endl;
        for (int i = 0; i < 26; i++) {
            cout << static_cast<char>('a' + i) << ' ';
        }
        cout << endl;
        for (int i = 0; i < 26; i++) {
            if (table.transform.count('a' + i))
                cout << static_cast<char>(table.transform.at('a' + i)) << ' ';
            else
                cout << "_ ";
        }
        cout << endl;

        bool ok = true;
        cout << endl << "partially decoded: " <<endl;
        string result;
        for (const auto& s: str) {
            if(!isalpha(s)) {
                result += s;
                continue;
            }
            char c = static_cast<char>(tolower(s));
            if (table.reverse.count(c))
                result += static_cast<char>(table.reverse.at(c));
            else {
                result += static_cast<char>(toupper(s));;
                ok = false;
            }
        }
        cout << result << endl << endl;

        if (ok) {
            cout << "The message has been fully decoded!" << endl;
            return table;
        }

        cout << "Input <UPPER> <lower> char to fill the table: " << endl;
        char from, to;
        cin >> to >> from;
        cin.ignore(32767, '\n');

        if(!isalpha(from) || !isalnum(to)) {
            cout << "input is not valid." << endl;
            continue;
        }

        from = static_cast<char>(tolower(from));
        to = static_cast<char>(tolower(to));

        table.transform[from] = to;
        table.reverse[to] = from;
    }
    return table;
}

bool Monoalphabetic::fillTable(ReplaceTable &table, const string& first, const string& second)
{
    if(first.length() != second.length()) return false;
    for (unsigned i = 0; i < first.length(); i++) {
        if (table.reverse.count(first.at(i)) && table.reverse[first.at(i)] != second.at(i)) {
            return false;
        }
        if (table.transform.count(second.at(i)) && table.transform[second.at(i)] != first.at(i)) {
            return false;
        }
    }
    for (unsigned i = 0; i < first.length(); i++) {
        table.reverse[first.at(i)] = second.at(i);
        table.transform[second.at(i)] = first.at(i);
    }
    return true;
}

bool Monoalphabetic::fillTable(ReplaceTable &table, int first, int second)
{
    if(table.reverse.count(first) && table.reverse[first] != second) return false;
    if(table.transform.count(second) && table.transform[second] != first) return false;
    table.reverse[first] = second;
    table.transform[second] = first;
    return true;
}

ReplaceTable Monoalphabetic::genTable()
{
    ReplaceTable table;
    int lastJ = 0;
    for (int i = 0; i < 26; i++) {
        int rand = Random::randomInt(i, 25);
        if (table.reverse.count(rand)) {
            for (int j = lastJ; j < i; j++) {
                if (table.reverse.count(j))
                    continue;
                table.transform[i] = j;
                table.reverse[j] = i;
                break;
            }
        } else {
            table.transform[i] = rand;
            table.reverse[rand] = i;
        }
    }
    return table;
}

ReplaceTable Monoalphabetic::genTable(const string &str)
{

    auto random = new Random(str);

    ReplaceTable table;
    int lastJ = 0;
    for (int i = 0; i < 26; i++) {
        int rand = random->next(i, 25);
        if (table.reverse.count(rand)) {
            for (int j = lastJ; j < i; j++) {
                if (table.reverse.count(j))
                    continue;
                table.transform[i] = j;
                table.reverse[j] = i;
                break;
            }
        } else {
            table.transform[i] = rand;
            table.reverse[rand] = i;
        }
    }
    return table;
}

string Monoalphabetic::encode(const string &str, const map<int, int>& table)
{
    string res;
    for (const auto& c : str) {
        if(!isalpha(c)) {
            res += c;
            continue;
        }
        int t = tolower(c) - 'a';
        t = table.at(t);
        res += static_cast<char>(t + 'a');
    }
    return res;
}

string Monoalphabetic::encodeWithTable(const string &str, const ReplaceTable &table)
{
    return encode(str, table.transform);
}

string Monoalphabetic::decodeWithTable(const string &str, const ReplaceTable &table)
{
    return encode(str, table.reverse);
}

ReplaceTable Monoalphabetic::decodeGuess(string str)
{
    ReplaceTable table;
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    map<char, int> freq;

    for (const auto& c : str) {
        if(!isalpha(c)) continue;
        freq[static_cast<char>(c)]++;
    }

    multimap<int, char> rfreq;

    for (const auto& c : freq) {
        rfreq.insert(make_pair(INT_MAX - c.second, c.first));
    }

    int i = 0;

    const char f_[] = {'e', 't', 'a', 'o', 'i', 'n', 's', 'h', 'r', 'd', 'l', 'u'};

    for (const auto& r: rfreq) {
        if(i < 3 && fillTable(table, r.second, f_[i])) {
            i++;
        }
    }

    stringstream ss(str);
    map<string, int> words;

    while (ss) {
        string tmp;
        ss >> tmp;
        if(tmp.length() > 3 || tmp.length() <= 0) continue;
        words[tmp]++;
    }

    list<pair<int, string>> rwords;

    for (const auto& f : words) {
        rwords.push_back(make_pair(INT_MAX - f.second, f.first));
    }

    rwords.sort();

    vector<string> fsub = { "the", "to", "of", "i", "a", "and", "is", "that", "in", "b"};

    vector<string> f1 = {"it", "be", "as", "at", "so", "we", "he", "by", "or", "on", "do", "if", "me", "my", "up", "an", "go", "no", "us", "am"};

    vector<string> f2 = {"for", "are", "but", "not", "you", "all", "any", "can", "had", "her", "was", "one", "our", "out", "day", "get", "has", "him", "his", "how", "man", "new", "now", "old", "see", "two", "way", "who", "boy", "did", "its", "let", "put", "say", "she", "too", "use", "bob"};

    ReplaceTable snapshot = table;

retry:
    table = snapshot;

    for (auto f = rwords.begin(); f != rwords.end(); f++) {
        bool found = false;
        for (unsigned i = 0; i < fsub.size(); i++) {
            if (fillTable(table, f->second, fsub[i])) {
                found = true;
                break;
            }
        }
        if (found || f->second.length() > 3) continue;
        if (f->second.length() == 2) {
            for (unsigned i = 0; i < f1.size(); i++) {
                if (fillTable(table, f->second, f1[i])) {
                    found = true;
                    break;
                }
            }
        } else if (f->second.length() == 3) {
            for (unsigned i = 0; i < f2.size(); i++) {
                if (fillTable(table, f->second, f2[i])) {
                    found = true;
                    break;
                }
            }
        }
        if(found != true && f->second.length() < 4) {
            std::swap(*f, *(f--));
            goto retry;
        }
    }

    return tryFillTable(str, table);
}
