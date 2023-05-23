
.PHONY: all test
all : 1.cpp trie.cpp
	@g++ 1.cpp trie.cpp -o p0.out -std=c++17
	@./p0.out

test : gen.cpp 1.cpp trie.cpp
	@g++ gen.cpp -std=c++17 -o gen.out && ./gen.out
	@g++ 1.cpp -std=c++17 trie.cpp -o p0.out && ./p0.out
	@diff my_result.txt ans.txt