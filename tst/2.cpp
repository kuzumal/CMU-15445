#include <bits/stdc++.h>

using namespace std;

class Parent {
public:
    int val;
    bool is_child;
    Parent() : is_child(false) {}
    Parent(int val) : val(val) { is_child = false; }
};

class Child : public Parent {
public:
    int val2;
    Child(int val2) : Parent(), val2(val2) { is_child = true; }
    Child(int val, int val2) : Parent(val), val2(val2) { is_child = true; }
};

int main(){
    Child ch(move(1), 2);
    return 0;
}
