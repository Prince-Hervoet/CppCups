#pragma once
#include <string.h>

typedef void (*Freer)(void* data);

typedef struct {
  char* key;
  void* data;
} CupSkipListData;

typedef struct {
  Freer freer;
} CupSkipListDataFreer;

typedef struct CupSkipListNode {
  CupSkipListData* cdata;
  double score;
  struct CupSkipListNode** levelsPtr;
} CupSkipListNode;

typedef struct {
  CupSkipListNode* head;
  CupSkipListNode* tail;
  int maxLevel;
  unsigned long size;
  Freer freer;
} CupSkipList;

CupSkipList* MakeCupSkipList();
void* Search(CupSkipList* sl, char* key);
void Insert(CupSkipList* sl, char* key, void* data, double score);
void FreeSkipList(CupSkipList* sl);
