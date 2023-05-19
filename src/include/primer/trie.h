#pragma once

#include <algorithm>
#include <cstddef>
#include <future>  // NOLINT
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace bustub {

/// A special type that will block the move constructor and move assignment operator. Used in TrieStore tests.
class MoveBlocked {
 public:
  explicit MoveBlocked(std::future<int> wait) : wait_(std::move(wait)) {}

  MoveBlocked(const MoveBlocked &) = delete;
  MoveBlocked(MoveBlocked &&that) noexcept {
    if (!that.waited_) {
      that.wait_.get();
    }
    that.waited_ = waited_ = true;
  }

  auto operator=(const MoveBlocked &) -> MoveBlocked & = delete;
  auto operator=(MoveBlocked &&that) noexcept -> MoveBlocked & {
    if (!that.waited_) {
      that.wait_.get();
    }
    that.waited_ = waited_ = true;
    return *this;
  }

  bool waited_{false};
  std::future<int> wait_;
};

// A TrieNode is a node in a Trie.
class TrieNode {
 public:
  // Create a TrieNode with no children.
  TrieNode() = default;

  // Create a TrieNode with some children.
  explicit TrieNode(std::map<char, std::shared_ptr<const TrieNode>> children) : children_(std::move(children)) {}

  virtual ~TrieNode() = default;

  // Clone returns a copy of this TrieNode. If the TrieNode has a value, the value is copied. The return
  // type of this function is a unique_ptr to a TrieNode.
  //
  // You cannot use the copy constructor to clone the node because it doesn't know whether a `TrieNode`
  // contains a value or not.
  //
  // Note: if you want to convert `unique_ptr` into `shared_ptr`, you can use `std::shared_ptr<T>(std::move(ptr))`.
  virtual auto Clone() const -> std::unique_ptr<TrieNode> { return std::make_unique<TrieNode>(children_); }  // ?

  // A map of children, where the key is the next character in the key, and the value is the next TrieNode.
  std::map<char, std::shared_ptr<const TrieNode>> children_;

  // Indicates if the node is the terminal node.
  bool is_value_node_{false};

  // You can add additional fields and methods here. But in general, you don't need to add extra fields to
  // complete this project.
};

// A TrieNodeWithValue is a TrieNode that also has a value of type T associated with it.
template <class T>
class TrieNodeWithValue : public TrieNode {
 public:
  // Create a trie node with no children and a value.
  explicit TrieNodeWithValue(std::shared_ptr<T> value) : value_(std::move(value)) { this->is_value_node_ = true; }

  // Create a trie node with children and a value.
  TrieNodeWithValue(std::map<char, std::shared_ptr<const TrieNode>> children, std::shared_ptr<T> value)
      : TrieNode(std::move(children)), value_(std::move(value)) {
    this->is_value_node_ = true;
  }

  // Override the Clone method to also clone the value.
  //
  // Note: if you want to convert `unique_ptr` into `shared_ptr`, you can use `std::shared_ptr<T>(std::move(ptr))`.
  auto Clone() const -> std::unique_ptr<TrieNode> override {
    return std::make_unique<TrieNodeWithValue<T>>(children_, value_);
  }

  // The value associated with this trie node.
  std::shared_ptr<T> value_;
};

// A Trie is a data structure that maps strings to values of type T. All operations on a Trie should not
// modify the trie itself. It should reuse the existing nodes as much as possible, and create new nodes to
// represent the new trie.
class Trie {
 private:
  // The root of the trie.
  std::shared_ptr<const TrieNode> root_{nullptr};

  // Create a new trie with the given root.
  explicit Trie(std::shared_ptr<const TrieNode> root) : root_(std::move(root)) {}

 public:
  // Create an empty trie.
  Trie() = default;

  // Get the value associated with the given key.
  // 1. If the key is not in the trie, return nullptr.
  // 2. If the key is in the trie but the type is mismatched, return nullptr.
  // 3. Otherwise, return the value.
  template <class T>
  auto Get(std::string_view key) const -> const T * {
    
    if (root_ == nullprt) return nullptr;
    if (key.length() == 0) { 
	if (root_->is_value_node == false) return nullptr;
	TrieNodeWithValue<T>* tnv = dynamic_cast<TrieNodeWithValue<T>> (root_.get()); 
	return (tnv->value).get();
    }
    std::shared_ptr<const TrieNode> head_(root_); int index_ = 0, size_ = key.length();

    while (true) {
      if (index_ == size_) {
	if (head_->is_value_node) {
	    TrieNodeWithValue<T>* tnv = dynamic_cast<TrieNodeWithValue<T>> (head_.get()); 
	    return (tnv->value).get();
	}
	else { return nullptr; }
      } 
      auto c = head_->children_.find(key.at(index_));

      if (c == head_.children_.end()) { return nullptr; }
      else {      
	BUSTUB_ENSURE(c != nullptr, "----***WRONG***----\n");

        std::shared_ptr<const TrieNode> ne_ = (*c).second;
        index_ ++;
        head_ = ne_;
      }
    }
  }

  // Put a new key-value pair into the trie. If the key already exists, overwrite the value.
  // Returns the new trie.
  template <class T>
  auto Put(std::string_view key, T value) const -> Trie {
    
    if (key.length() == 0) {
      std::shared_ptr<const TrieNode> cp = std::shared_ptr<const TrieNode>(std::move(root_->Clone()));
      TrieNodeWithValue<T> end_(cp->children_, std::make_shared(value));
      *root_ = std::move(end_); 
      return Trie(root); 
    }

    int len = key.length(), match = 0;
    std::shared_ptr<TrieNode> new_rt1(new TrieNode);
    std::shared_ptr<TrieNode> new_rt2(new TrieNodeWithValue);
    std::shared_ptr<TrieNode> new_rt = root_->is_value_node_ == true ? new_rt2 : new_rt1, tp_rt(new_rt);
    const TrieNode* fd_rt = root_.get();


     while (len != match) {
      
	if (root_->is_value_node) {
	    TrieNodeWithValue<TrieNodeWithValue<T>>* tnv1 = dynamic_cast<TrieNodeWithValue *> (tp_rt.get()),
			    			   * tnv2 = dynamic_cast<TrieNodeWithValue *> (fd_rt);
	    *tnv1 = std::move(TrieNodeWithValue<T> (tnv2->children_, tnv2->value_));
	}
	else { *tp_rt = std::move(TrieNode(tnv2->children_)); }

	TrieNode* ne = fd_rt->children_[key.at(match)].get();
	if (ne->is_value_node_) {
	    std::shared_ptr<TrieNode> me(new TrieNodeWithValue<T>);
	    tp_rt->children_[key.at(match)] = me;
	}
	else {
	    std::shared_ptr<TrieNode> me(new TrieNode);
	    tp_rt->children_[key.at(match)] = me;
	}
	tp_rt = tp_rt->children_[key.at(match)];
	fd_rt = fd_rt->children_[key.at(match)].get();
	match ++;
    }
    
    TrieNodeWithValue<TrieNodeWithValue<T>>* tnv = dynamic_cast<TrieNodeWithValue *> (tp_rt.get());
    tnv->value_ = std::move(value);
    
    this->root_ = new_rt;
    return std::move(Trie(this->root_));
}

  // Remove the key from the trie. If the key does not exist, return the original trie.
  // Otherwise, returns the new trie.
  auto Remove(std::string_view key) const -> Trie {

    if (this->Get(key) == nullptr) return std::move(Trie(root_));

    if (key.length() == 0) {
      *root_ = std::move(TrieNode(root_->children_); 
      return Trie(root); 
    }

    int len = key.length(), match = 0;
    std::shared_ptr<TrieNode> new_rt1(new TrieNode);
    std::shared_ptr<TrieNode> new_rt2(new TrieNodeWithValue);
    std::shared_ptr<TrieNode> new_rt = root_->is_value_node_ == true ? new_rt2 : new_rt1, tp_rt(new_rt);
    const TrieNode* fd_rt = root_.get();


     while (len != match) {
      
	if (root_->is_value_node) {
	    TrieNodeWithValue<TrieNodeWithValue<T>>* tnv1 = dynamic_cast<TrieNodeWithValue *> (tp_rt.get()),
			    			   * tnv2 = dynamic_cast<TrieNodeWithValue *> (fd_rt);
	    *tnv1 = std::move(TrieNodeWithValue<T> (tnv2->children_, tnv2->value_));
	}
	else { *tp_rt = std::move(TrieNode(tnv2->children_)); }

	TrieNode* ne = fd_rt->children_[key.at(match)].get();
	if (ne->is_value_node_ && match != len - 1) {
	    std::shared_ptr<TrieNode> me(new TrieNodeWithValue<T>);
	    tp_rt->children_[key.at(match)] = me;
	}
	else {
	    std::shared_ptr<TrieNode> me(new TrieNode);
	    tp_rt->children_[key.at(match)] = me;
	}
	tp_rt = tp_rt->children_[key.at(match)];
	fd_rt = fd_rt->children_[key.at(match)].get();
	match ++;
    }
    
    TrieNodeWithValue<TrieNodeWithValue<T>>* tnv = dynamic_cast<TrieNodeWithValue *> (tp_rt.get());
    tnv->value_ = std::move(value);
    
    this->root_ = new_rt;
    return std::move(Trie(this->root_));
  }
};

}  // namespace bustub

