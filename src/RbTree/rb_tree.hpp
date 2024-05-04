#ifndef _RADIX_TREE_H_
#define _RADIX_TREE_H_

#include <utility>
const char kRed = 1;
const char kBlack = 2;

template <typename K, typename V>
class RbTreeNode {
  using RbTreeNodePtr = RbTreeNode*;

 public:
  RbTreeNode() {}
  RbTreeNode(K& key, V& value) : key(key), value(value) {}
  RbTreeNode(K&& key, V&& value)
      : key(std::forward<K>(key)), value(std::forward<V>(value)) {}

 public:
  char color;
  RbTreeNodePtr left;
  RbTreeNodePtr right;
  RbTreeNodePtr parent;
  K key;
  V value;

 public:
  RbTreeNodePtr getGrandparnet() {
    if (this->parent == nullptr) return nullptr;
    return this->parent->parent;
  }

  RbTreeNodePtr getUncle() {
    RbTreeNodePtr grandparent = getGrandparnet();
    if (grandparent == nullptr) return nullptr;
    if (this->parent == grandparent->left)
      return grandparent->right;
    else
      return grandparent->left;
  }
};

template <typename K, typename V>
class RbTree {
  using RbTreeNodeType = RbTreeNode<K, V>;
  using RbTreeNodePtr = RbTreeNode<K, V>*;
  using ComparerFunc = int (*)(const K& k1, const K& k2);

 public:
  template <typename K2 = K, typename V2 = V>
  void Insert(K2&& key, V2&& value) {
    RbTreeNodePtr nNode =
        new RbTreeNode<K2, V2>(std::forward<K2>(key), std::forward<V2>(value));
    if (root == nullptr)
      root = nNode;
    else if (!insert(nNode))
      return;
    insertJudge(nNode);
  }

 private:
  RbTreeNodePtr root;
  ComparerFunc comparer;
  bool is_multiple = false;

 public:
  RbTree() {}
  RbTree(ComparerFunc comparer) {}

 private:
  void rightRotate(RbTreeNodePtr node) {
    RbTreeNodePtr grandparent = node->getGrandparnet();
    RbTreeNodePtr parent = node->parent;
    RbTreeNodePtr right = node->right;
    if (parent == nullptr) return;
    if (right) {
      parent->left = right;
      right->parent = parent;
      node->right = parent;
    }
    if (parent == root) root = node;
    node->parent = grandparent;
    if (grandparent) {
      if (grandparent->left == parent)
        grandparent->left = node;
      else
        grandparent->right = node;
    }
  }

  void leftRotate(RbTreeNodePtr node) {
    RbTreeNodePtr grandparent = node->getGrandparnet();
    RbTreeNodePtr parent = node->parent;
    RbTreeNodePtr left = node->left;
    if (parent == nullptr) return;
    if (left) {
      parent->right = left;
      left->parent = parent;
      node->left = parent;
    }
    if (parent == root) root = node;
    node->parent = grandparent;
    if (grandparent) {
      if (grandparent->left == parent)
        grandparent->left = node;
      else
        grandparent->right = node;
    }
  }

  bool insert(RbTreeNodePtr node) {
    RbTreeNodePtr run = root;
    RbTreeNodePtr insertPoint = nullptr;
    while (run) {
      int result = comparer(node->key, run->key);
      if (result < 0) {
        run = run->left;
      } else if (result > 0) {
        run = run->right;
      } else {
        if (is_multiple)
          run = run->left;
        else
          return false;
      }
      insertPoint = run->parent;
    }
    if (insertPoint->left)
      insertPoint->right = node;
    else
      insertPoint->left = node;
    node->parent = insertPoint;
    return true;
  }

  void insertJudge(RbTreeNodePtr target) {
    if (target == nullptr) return;
    if (target->parent == nullptr)
      target->color = kBlack;
    else
      insertCaseOne();
  }

  void insertCaseOne(RbTreeNodePtr node) {
    if (node->parent->color == kBlack)
      return;
    else
      insertCaseTwo(node);
  }

  void insertCaseTwo(RbTreeNodePtr node) {
    RbTreeNodePtr uncle = node->getUncle();
    if (uncle != nullptr && uncle->color == kRed) {
      node->parent->color = kBlack;
      uncle->color = kBlack;
      RbTreeNodePtr grandparent = node->getGrandparnet();
      if (grandparent) grandparent->color = kRed;
      insertJudge(grandparent);
    } else
      insertCaseThree(node);
  }

  void insertCaseThree(RbTreeNodePtr node) {
    RbTreeNodePtr grandparent = node->getGrandparnet();
    RbTreeNodePtr parent = node->parent;
    if (node == parent->right && node->parent == grandparent->left) {
      leftRotate(node);
      node = node->left;
    } else if (node == parent->left && parent == grandparent->right) {
      rightRotate(node);
      node = node->right;
    }
    insertCaseFour(node);
  }

  void insertCaseFour(RbTreeNodePtr node) {
    RbTreeNodePtr grandparent = node->getGrandparnet();
    RbTreeNodePtr parent = node->parent;
    grandparent->color = kRed;
    parent->color = kBlack;
    if (node == parent->left && parent == grandparent->left) {
      rightRotate(parent);
    } else {
      leftRotate(parent);
    }
  }
};

#endif