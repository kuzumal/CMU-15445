#include <bits/stdc++.h>
using namespace std;

#define G() (pair<string, int>{Gstring(), Gdata()})

const int data_limit = 1000, len_limit = 5;
int data_number = 100000;
map<string, int> m;

int Gdata() { return rand() % data_limit; }

string Gstring() {
    int len = rand() % len_limit + 1;
    string s;
    while (len --) { s += '0' + rand() % 10; }
    return s;
}

int main() {
    srand(time(NULL));
    ofstream test("./test.txt", ios::out | ios::trunc),
    ans("./ans.txt", ios::out | ios::trunc);
    while (data_number --) {
        pair<string, int> temp = G();
        m.insert(temp);
        test << "P " << temp.first << " " << temp.second << endl;
        if (rand() % 3) {
            test << "S " << temp.first << endl;
            ans  << temp.second << endl;
        }
        if (rand() % 5) {
            auto [f, s] = (*m.begin());
            m.erase(f);
            test << "R " << f << endl;
        }
    }

    return 0;
}