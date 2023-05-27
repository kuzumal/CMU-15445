#include <bits/stdc++.h>
#include "./trie.h"
#include <string_view>

using namespace bustub;

#define de(X) { std::cout << #X << " : " << (X) << std::endl; }

#define ASSERT_EQ(a, b) assert((a) == (b))

#define P(root, key, value) { root = std::make_shared<Trie>(std::move(root->Put(key, value))); }
#define S(root, type, key) (root->Get<type>(key))
#define R(root, key) { root = std::make_shared<Trie>(std::move(root->Remove(key))); }

int main() {
    std::shared_ptr<Trie> root(new Trie);
    // std::ifstream test("./test.txt", std::ios::in);
    // std::ofstream ans ("./my_result.txt", std::ios::out | std::ios::trunc);

    // std::string mode, key, s; int val;
    // while (std::getline(test, s)) { 
    //     std::stringstream line(s);
    //     line >> mode >> key >> val;
    //     if (mode[0] == 'P') { P(root, key, val); }
    //     else if (mode[0] == 'R') { R(root, key); }
    //     else {
    //         const int* temp = S(root, int, key);
    //         assert(temp != nullptr);
    //         ans << *temp << std::endl;
    //     }
    // }
    // test.close(); ans.close();

    P(root, "123", 123);
    P(root, "12", 12);

    R(root, "12");
    
    R(root, "123");
    ASSERT_EQ(S(root, int, "12"), nullptr);
    // auto trie = Trie();
    // trie = trie.Put<uint32_t>("test", 2333);
    // trie = trie.Put<uint32_t>("te", 23);
    // trie = trie.Put<uint32_t>("tes", 233);


    // trie = trie.Remove("te");
    // ASSERT_EQ(trie.Get<uint32_t>("te"), nullptr);
    // trie = trie.Remove("tes");
    // ASSERT_EQ(*trie.Get<uint32_t>("te"), 23);
    // ASSERT_EQ(trie.Get<uint32_t>("tes"), nullptr);
    // trie = trie.Remove("test");
    // ASSERT_EQ(trie.Get<uint32_t>("test"), nullptr);

    // ASSERT_EQ(trie.Get<uint32_t>("te"), nullptr);
    // ASSERT_EQ(trie.Get<uint32_t>("tes"), nullptr);
    // ASSERT_EQ(trie.Get<uint32_t>("test"), nullptr);
    return 0; 
}
