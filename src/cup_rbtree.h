#pragma once

#include <string.h>
#define RED 1
#define BLACK 0
#define bool char

typedef int (*Compare)(void*, void*);
typedef void (*Freer)(void*);

typedef struct {
  CupRbTreeNode* parent;
  CupRbTreeNode* left;
  CupRbTreeNode* right;
  void* data;
  bool color;
} CupRbTreeNode;

typedef struct {
  CupRbTreeNode* root;
  Compare cmp;
  Freer freer;
  int size;
} CupRbTree;

void Insert(CupRbTree* tree, void* data);
void insertAction(CupRbTree* tree, CupRbTreeNode* parent, CupRbTreeNode* node);
void Remove(void* data);

static inline CupRbTreeNode* getParent(CupRbTreeNode* node);
static inline CupRbTreeNode* getUncle(CupRbTreeNode* node);
static inline CupRbTreeNode* getGrandparent(CupRbTreeNode* node);
static inline void changeColor(CupRbTreeNode* node);

void leftRotate(CupRbTreeNode* node);
void rightRotate(CupRbTreeNode* node);
void insertCase1(CupRbTreeNode* node);
void insertCase2(CupRbTreeNode* node);
void insertCase3(CupRbTreeNode* node);
void insertCase4(CupRbTreeNode* node);
void insertCase5(CupRbTreeNode* node);