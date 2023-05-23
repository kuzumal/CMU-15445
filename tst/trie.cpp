#include "./trie.h"
#include <string_view>
#include <bits/stdc++.h>

#define de(X) { std::cout << #X << " : " << X << std::endl; }

namespace bustub {

template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
  
  if (root_ == nullptr) { return nullptr; }
  if (key.length() == 0) { 
    if (root_->is_value_node_ == false) { return nullptr; }
    if (const TrieNodeWithValue<T>* tnv = dynamic_cast<const TrieNodeWithValue<T> *> (root_.get()))
      { 
        if (tnv->value_)  { return (tnv->value_).get(); }
        else { return nullptr; } 
      }
    else { return nullptr; }
  }
  std::shared_ptr<const TrieNode> head_(root_);
  int index_ = 0, size_ = key.length();

  while (true) {
    if (index_ == size_) {
      if (head_->is_value_node_) {
        if (const TrieNodeWithValue<T>* tnv = dynamic_cast<const TrieNodeWithValue<T> *> (head_.get()))
          { 
            if (tnv->value_)  { return (tnv->value_).get(); }
            else { return nullptr; } 
          }
        else { return nullptr; }
      }
      else { return nullptr; }
    } 
    if (head_->children_.count(key.at(index_)) == 0) { return nullptr; }
    else { head_ = head_->children_.at(key.at(index_ ++)); }
  }
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {

  if (key.length() == 0) {
    std::shared_ptr<TrieNode> 
      new_rt = 
        root_ == nullptr ?
        std::make_shared<TrieNodeWithValue<T>>(std::move(TrieNodeWithValue<T>(std::make_shared<T>(std::move(value)))))
      : std::make_shared<TrieNodeWithValue<T>>(std::move(TrieNodeWithValue<T>(root_->children_, std::make_shared<T>(std::move(value)))));
    return std::move(Trie(new_rt));
  }

  int len = key.length(), match = 0;
  std::shared_ptr<TrieNode> new_rt = 
    root_ == nullptr ? 
      std::make_shared<TrieNode>(std::move(TrieNode()))
    : std::shared_ptr<TrieNode>(std::move(root_->Clone())),
  tp_rt(new_rt);
  std::shared_ptr<const TrieNode> fd_rt(root_);

  while (len != match) {
    if (fd_rt == nullptr || fd_rt->children_.count(key.at(match)) == 0) {
       while (len - 1 != match) {
          std::shared_ptr<TrieNode> temp(new TrieNode);
          tp_rt->children_[key.at(match ++)] = temp;
          tp_rt = temp;
       }
      std::shared_ptr<TrieNodeWithValue<T>> 
        temp(new TrieNodeWithValue<T>(std::make_shared<T>(std::move(value))));
      tp_rt->children_[key.at(match)] = temp; 
      break;
    }
    fd_rt = fd_rt->children_.at(key.at(match));
    if (len - 1 == match) {
      std::shared_ptr<TrieNode> 
        end_ = std::make_shared<TrieNodeWithValue<T>>(std::move(TrieNodeWithValue<T> (fd_rt->children_, std::make_shared<T>(std::move(value)))));
      tp_rt->children_[key.at(match)] = end_; 
      break;
    }
    std::shared_ptr<TrieNode> temp = std::shared_ptr<TrieNode>(std::move(fd_rt->Clone()));
    tp_rt->children_[key.at(match)] = temp;
    tp_rt = temp;
    match ++;
  }

	return std::move(Trie(new_rt));
}

auto Trie::Remove(std::string_view key) const -> Trie {

  if (root_ == nullptr) { return std::move(Trie(root_)); }
  if (key.length() == 0) {
	  if (root_->is_value_node_ == false) { return std::move(Trie(root_)); }
    else {
      std::shared_ptr<TrieNode> nrt(new TrieNode);
		  *nrt = std::move(TrieNode(root_->children_)); 
      return std::move(Trie(nrt)); 
	  }
	}

  int len = key.length(), match = 0;
  std::shared_ptr<TrieNode> new_rt = std::shared_ptr<TrieNode>(std::move(root_->Clone()));
  new_rt = std::shared_ptr<TrieNode>(std::move(new_rt->Clone()));
  std::shared_ptr<TrieNode> tp_rt(new_rt);
  std::shared_ptr<const TrieNode> fd_rt(root_); 

  while (len != match) {
    if (fd_rt->children_.count(key.at(match)) == 0) { return std::move(Trie(root_)); }
    fd_rt = (fd_rt->children_).at(key.at(match));
    if (len - 1 == match) {
      if (!(fd_rt->is_value_node_)) { return std::move(Trie(root_)); }
      std::shared_ptr<TrieNode> temp = std::make_shared<TrieNode>(std::move(TrieNode(fd_rt->children_)));
      tp_rt->children_[key.at(match)] = temp;
      return std::move(Trie(new_rt));
    }
    std::shared_ptr<TrieNode> temp = std::move(std::shared_ptr<TrieNode>(std::move(fd_rt->Clone())));
    tp_rt->children_[key.at(match)] = temp;
    tp_rt = temp;
    match ++; 
  }
  assert(0);
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
