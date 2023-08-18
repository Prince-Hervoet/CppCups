#pragma once

typedef struct {
  char* key;
  void* data;
} CupSkipListData;

typedef struct {
  CupSkipListData* cdata;
  double score;
  CupSkipListNode** levelsPtr;
} CupSkipListNode;

typedef struct {
  /* data */
  CupSkipListNode* head;
  CupSkipListNode* tail;
  int maxLevel;
  unsigned long size;
} CupSkipList;

CupSkipList* MakeCupSkipList();
void* Search(CupSkipList* sl, char* key);
void Insert(CupSkipList* sl, char* key, void* data, double score);
