#include "cup_skiplist.h"

#include <stdlib.h>
#include <time.h>

#define SKIPLIST_MAX_LEVEL 32
#define SKIPLIST_LEVEL_FLAG 0.25

static CupSkipListNode* createNode(int level) {
  CupSkipListNode* node = (CupSkipListNode*)malloc(sizeof(CupSkipListNode));
  node->score = 0.0;
  node->cdata = NULL;
  node->levelsPtr = (CupSkipListNode**)malloc(sizeof(CupSkipListNode*) * level);
  for (int i = 0; i < level; i++) {
    node->levelsPtr[i] = NULL;
  }
  return node;
}

static int getRandomLevel() {
  int level = 1;
  srand((unsigned)time(NULL));
  while ((rand() & 0xFFFF) < (SKIPLIST_LEVEL_FLAG * 0xFFFF)) level += 1;
  return (level < SKIPLIST_MAX_LEVEL) ? level : SKIPLIST_MAX_LEVEL;
}

static CupSkipListData* listSearch(CupSkipList* sl, char* key) {
  if (sl->size == 0 || sl == NULL || key == NULL) {
    return NULL;
  }
  CupSkipListNode* run = sl->head;
  CupSkipListNode* nextNode = NULL;
  int res = 0;
  for (int i = sl->maxLevel; i >= 0; i--) {
    nextNode = run->levelsPtr[i];
    while (nextNode != NULL) {
      res = strcmp(nextNode->cdata->key, key);
      if (res > 0) {
        break;
      } else if (res < 0) {
        run = nextNode;
      } else {
        return nextNode->cdata;
      }
    }
  }
  return NULL;
}

static void freeSkipListNode(CupSkipList* sl, CupSkipListNode* node) {
  if (node == NULL) {
    return;
  }
  free(node->levelsPtr);
  free(node->cdata->key);
  sl->freer(node->cdata->data);
  free(node);
}

CupSkipList* MakeCupSkipList() {
  // 初始化头节点
  CupSkipListNode* headNode = createNode(SKIPLIST_MAX_LEVEL);
  CupSkipList* list = (CupSkipList*)malloc(sizeof(CupSkipList));
  list->maxLevel = 0;
  list->size = 0;
  list->head = headNode;
  list->tail = headNode;
  return list;
}

void* Search(CupSkipList* sl, char* key) {
  CupSkipListData* cd = listSearch(sl, key);
  if (cd != NULL) {
    return cd->data;
  }
  return NULL;
}

void Insert(CupSkipList* sl, char* key, void* data, double score) {
  if (key == NULL) return NULL;
  int newLevel = getRandomLevel();
  sl->maxLevel = sl->maxLevel > newLevel ? sl->maxLevel : newLevel;
  CupSkipListNode *run = sl->head, *nextNode = NULL,
                  *node = createNode(newLevel);
  CupSkipListNode* needUpdateNodes[newLevel];
  int res = 0;
  for (int i = sl->maxLevel - 1; i >= 0; i--) {
    nextNode = run->levelsPtr[i];
    while (nextNode != NULL) {
      res = strcmp(nextNode->cdata->key, key);
      if (res > 0) {
        break;
      } else if (res < 0) {
        run = nextNode;
      } else {
        sl->freer(nextNode->cdata->data);
        nextNode->cdata->data = data;
        freeSkipListNode(sl, node);
        return;
      }
    }
    if (i < newLevel) {
      needUpdateNodes[i] = run;
    }
  }
  for (int i = 0; i < newLevel; i++) {
    node->levelsPtr[i] = run->levelsPtr[i];
    run->levelsPtr[i] = node;
  }
}

void FreeSkipList(CupSkipList* sl) {
  if (sl == NULL) {
    return;
  }
  CupSkipListNode* run = sl->head;
  CupSkipListNode* nextNode = run->levelsPtr[0];
  while (nextNode != NULL) {
    run = nextNode->levelsPtr[0];
    freeSkipListNode(sl, nextNode);
  }
  free(sl);
}
