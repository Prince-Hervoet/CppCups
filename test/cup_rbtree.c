#include "cup_rbtree.h"

#include <stdlib.h>

static inline CupRbTreeNode* getParent(CupRbTreeNode* node) {
  if (node == NULL) reutrn NULL;
  return node->parent;
}

static inline CupRbTreeNode* getGrandparent(CupRbTreeNode* node) {
  if (node == NULL) return NULL;
  if (node->parent == NULL) return NULL;
  return node->parent->parent;
}

static inline CupRbTreeNode* getUncle(CupRbTreeNode* node) {
  if (node == NULL) return NULL;
  CupRbTreeNode* nodeParent = getParent(node);
  CupRbTreeNode* nodeGrandparnet = getParent(nodeParent);
  if (nodeGrandparnet == NULL) return NULL;
  if (nodeGrandparnet->left == nodeParent) {
    return nodeGrandparnet->right;
  }
  return nodeGrandparnet->left;
}

static inline void changeColor(CupRbTreeNode* node) {
  if (node == NULL) return;
  if (node->color == RED) {
    node->color = BLACK;
  } else {
    node->color = RED;
  }
}

void Insert(CupRbTree* tree, void* data) {
  if (tree == NULL || data == NULL) return;
  CupRbTreeNode* run = tree->root;
  CupRbTreeNode* prev = NULL;
  bool isLeft = 1;
  int cmpRes = 0;
  while (run != NULL) {
    cmpRes = tree->cmp(data, run->data);
    if (cmpRes > 0) {
      // data比run->data大，往右边走
      prev = run;
      run = run->right;
      isLeft = 0;
    } else if (cmpRes < 0) {
      prev = run;
      run = run->left;
      isLeft = 1;
    } else {
      break;
    }
  }

  if (run == NULL) {
    // 如果run为NULL，就说明要插入新的节点
    CupRbTreeNode* node = (CupRbTreeNode*)malloc(sizeof(CupRbTreeNode));
    node->color = RED;
    node->left = NULL;
    node->right = NULL;
    node->data = data;
    node->parent = NULL;

    if (prev != NULL) {
      if (isLeft) {
        prev->left = node;
      } else {
        prev->right = node;
      }
      node->parent = prev;
    } else {
      tree->root = node;
    }
    insertCase1(node);
  } else {
    // 更新一下
    tree->freer(run->data);
    run->data = data;
  }
}

void insertCase1(CupRbTreeNode* node) {
  if (node->parent == NULL) {
    node->color = BLACK;
  } else {
    insertCase2(node);
  }
}

void insertCase2(CupRbTreeNode* node) {
  if (node->parent->color == BLACK) {
    return;
  } else {
    insertCase3(node);
  }
}

void insertCase3(CupRbTreeNode* node) {
  CupRbTreeNode* uncle = getUncle(node);
  if (uncle != NULL && uncle->color == RED) {
    node->parent->color = BLACK;
    uncle->color = BLACK;
    CupRbTreeNode* grandparent = getGrandparent(node);
    grandparent->color = RED;
    insertCase1(grandparent);
  } else {
    insertCase4(node);
  }
}

void insertCase4(CupRbTreeNode* node) {
  CupRbTreeNode* grandparent = getGrandparent(node);
  if ((node == node->parent->right) && (node->parent == grandparent->left)) {
    leftRotate(node);
    node = node->left;
  } else if ((node == node->parent->left) &&
             (node->parent == grandparent->right)) {
    rightRotate(node);
    node = node->right;
  }
  insertCase5(node);
}

void insertCase5(CupRbTreeNode* node) {
  CupRbTreeNode* grandparent = getGrandparent(node);
  node->parent->color = BLACK;
  grandparent->color = RED;
  if (node == node->parent->left && node->parent == grandparent->left) {
    rightRotate(node->parent);
  } else {
    leftRotate(node->parent);
  }
}

void leftRotate(CupRbTreeNode* node) {
  if (node == NULL) return;
  CupRbTreeNode* rightChild = node->right;
  CupRbTreeNode* rightChildLeftChild = rightChild->left;
  CupRbTreeNode* nodeParent = getParent(node);

  rightChild->left = node;
  node->right = rightChildLeftChild;
  if (nodeParent != NULL) {
    bool isLeftChild = (nodeParent->left == node);
    if (isLeftChild) {
      nodeParent->left = rightChild;
    } else {
      nodeParent->right = rightChild;
    }
  }
}

void rightRotate(CupRbTreeNode* node) {
  if (node == NULL) return;
  CupRbTreeNode* leftChild = node->left;
  CupRbTreeNode* leftChildRightChild = leftChild->right;
  CupRbTreeNode* nodeParent = getParent(node);

  leftChild->right = node;
  node->left = leftChildRightChild;
  if (nodeParent != NULL) {
    bool isLeftChild = (nodeParent->left == node);
    if (isLeftChild) {
      nodeParent->left = leftChild;
    } else {
      nodeParent->right = leftChild;
    }
  }
}