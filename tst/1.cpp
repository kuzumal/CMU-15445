#include <bits/stdc++.h>
#include "./trie.h"
#include <string_view>

using namespace bustub;

int main() {
    std::shared_ptr<Trie> root(new Trie);
    std::map<int, int> m{std::pair<int, int>{1, 2}};
    std::cout << m.count(2) << std::endl;
    return 0; 
}
