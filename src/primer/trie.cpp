#include "../include/primer/trie.h"
#include <bits/stdc++.h>
#include <string_view>

#define de(X) \
  { std::cout << #X << " : " << (X) << std::endl; }

namespace bustub {

template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
  std::cout << "Runing ..\n";

  if (root_ == nullptr) {
    return nullptr;
  }
  std::shared_ptr<const TrieNode> tp_rt(root_);

  int match = 0;
  int len = key.length();

  while (true) {
    if (match == len) {
      if (!(tp_rt->is_value_node_)) {
        return nullptr;
      }
      if (const auto tnv = dynamic_cast<const TrieNodeWithValue<T> *>(tp_rt.get())) {
        return tnv->value_ == nullptr ? nullptr : (tnv->value_).get();
      }
      return nullptr;
    }
    if (tp_rt->children_.count(key.at(match)) == 0) {
      return nullptr;
    }
    tp_rt = tp_rt->children_.at(key.at(match++));
  }
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
  if (key.length() == 0) {
    std::shared_ptr<TrieNode> new_rt = root_ == nullptr
                                           ? std::make_shared<TrieNodeWithValue<T>>(
                                                 std::move(TrieNodeWithValue<T>(std::make_shared<T>(std::move(value)))))
                                           : std::make_shared<TrieNodeWithValue<T>>(std::move(TrieNodeWithValue<T>(
                                                 root_->children_, std::make_shared<T>(std::move(value)))));
    return Trie(new_rt);
  }

  int len = key.length();
  int match = 0;
  std::shared_ptr<TrieNode> new_rt =
      root_ == nullptr ? std::make_shared<TrieNode>(TrieNode()) : std::shared_ptr<TrieNode>(root_->Clone());
  std::shared_ptr<TrieNode> tp_rt(new_rt);
  std::shared_ptr<const TrieNode> fd_rt(root_);

  while (len != match) {
    if (fd_rt == nullptr || fd_rt->children_.count(key.at(match)) == 0) {
      while (len - 1 != match) {
        std::shared_ptr<TrieNode> temp(new TrieNode);
        tp_rt->children_[key.at(match++)] = temp;
        tp_rt = temp;
      }
      std::shared_ptr<TrieNodeWithValue<T>> temp(new TrieNodeWithValue<T>(std::make_shared<T>(std::move(value))));
      tp_rt->children_[key.at(match)] = temp;
      break;
    }
    fd_rt = fd_rt->children_.at(key.at(match));
    if (len - 1 == match) {
      std::shared_ptr<TrieNode> ed = std::make_shared<TrieNodeWithValue<T>>(
          std::move(TrieNodeWithValue<T>(fd_rt->children_, std::make_shared<T>(std::move(value)))));
      tp_rt->children_[key.at(match)] = ed;
      break;
    }
    std::shared_ptr<TrieNode> temp = std::shared_ptr<TrieNode>(fd_rt->Clone());
    tp_rt->children_[key.at(match)] = temp;
    tp_rt = temp;
    match++;
  }

  return Trie(new_rt);
}

auto Trie::Remove(std::string_view key) const -> Trie {
  if (root_ == nullptr) {
    return Trie(root_);
  }
  if (key.length() == 0) {
    if (!(root_->is_value_node_)) {
      return Trie(root_);
    }
    std::shared_ptr<TrieNode> nrt(new TrieNode);
    *nrt = TrieNode(root_->children_);
    return Trie(nrt);
  }

  int len = key.length();
  int match = 0;
  std::shared_ptr<TrieNode> new_rt = std::shared_ptr<TrieNode>(root_->Clone());
  new_rt = std::shared_ptr<TrieNode>(new_rt->Clone());
  std::shared_ptr<TrieNode> tp_rt(new_rt);
  std::shared_ptr<const TrieNode> fd_rt(root_);

  while (len != match) {
    if (fd_rt->children_.count(key.at(match)) == 0) {
      return Trie(root_);
    }
    fd_rt = (fd_rt->children_).at(key.at(match));
    if (len - 1 == match) {
      if (!(fd_rt->is_value_node_)) {
        return Trie(root_);
      }
      std::shared_ptr<TrieNode> temp = std::make_shared<TrieNode>(TrieNode(fd_rt->children_));
      tp_rt->children_[key.at(match)] = temp;
      return Trie(new_rt);
    }
    std::shared_ptr<TrieNode> temp = std::shared_ptr<TrieNode>(fd_rt->Clone());
    tp_rt->children_[key.at(match)] = temp;
    tp_rt = temp;
    match++;
  }
  assert(0);
}

auto Trie::Count() const -> uint32_t {
  if (root_ == nullptr) {
    return 0;
  }
  int c = root_->is_value_node_ ? 1 : 0;
  for (auto &p : root_->children_) {
    c += (Trie(p.second)).Count();
  }
  return c;
}

auto Trie::Countkey(std::string_view key) const -> uint32_t {
  if (root_ == nullptr) {
    return 0;
  }
  int c = root_->is_value_node_ ? 1 : 0;
  if (key.length() != 0) {
    if (root_->children_.count(key[0]) == 0) {
      return 0;
    }
    auto r = root_->children_.at(key[0]);
    key.remove_prefix(1);
    return (Trie(r)).Countkey(key);
  }

  for (auto &p : root_->children_) {
    c += (Trie(p.second)).Count();
  }
  return c;
}
// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// by the linker.

template auto Trie::Put(std::string_view key, int value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const int *;

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t *;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t *;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string *;

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer *;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
