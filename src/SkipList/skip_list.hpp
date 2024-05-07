#ifndef _SKIP_LIST_H_
#define _SKIP_LIST_H_
#include <cassert>
#include <random>
#include <vector>
#define MAX_LEVEL 12
static std::random_device rd;

static inline char getRandomLevel() {
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 10);
  char level = 0;
  while (dis(gen) <= 4) level++;
  if (level > MAX_LEVEL) level = MAX_LEVEL;
  return level;
}

template <typename K, typename V>
class SkipListNode {
 public:
  using SkipListNodePtr = SkipListNode*;

 public:
  SkipListNodePtr* next_nodes;
  unsigned char level = 0;
  K key;
  V value;

 public:
  SkipListNode() {}
  SkipListNode(char level) : level(level) {
    next_nodes = new SkipListNodePtr[level + 1];
  }

  SkipListNode(char level, K key) : key(key) {}

  ~SkipListNode() {
    if (next_nodes) delete[] next_nodes;
  }
};

template <typename K, typename V>
class SkipList {
 public:
  using SkipListNodePtr = SkipListNode<K, V>*;
  using Compartor = int (*)(const K& k1, const K& k2);
  using ValueType = V;
  using ValuePtr = V*;

 private:
  SkipListNodePtr head;
  unsigned char max_level = 0;
  Compartor compartor_func = nullptr;
  unsigned int size = 0;

 public:
  SkipList(Compartor compartor_func) : compartor_func(compartor_func) {
    assert(compartor_func != nullptr);
    head = new SkipListNode<K, V>(MAX_LEVEL);
  }

  ~SkipList() {
    if (head) delete head;
  }

 public:
  void Put(const K& key);
  bool Contains(const K& key);
  ValuePtr Get(const K& key);
  void Remove(const K& key);
  void Clear();

 private:
  SkipListNodePtr createNode(char level) {
    return new SkipListNode<K, V>(level);
  }

  SkipListNodePtr createNode(char level, const K& key) {
    return new SkipListNode<K, V>(level, key);
  }
};

template <typename K, typename V>
void SkipList<K, V>::Put(const K& key) {
  char need_level = getRandomLevel();
  max_level = max_level > need_level ? max_level : need_level;
  SkipListNodePtr run = head;
  SkipListNodePtr next = nullptr;
  std::vector<SkipListNodePtr> need_update;
  for (char i = max_level; i >= 0; --i) {
    while (run) {
      next = run->next_nodes[i];
      if (next == nullptr) break;
      int res = compartor_func(key, next->key);
      if (res > 0)
        run = next;
      else if (res < 0)
        break;
      else
        return;
    }
    if (i > need_level) continue;
    need_update.push_back(run);
  }
  SkipListNodePtr nNode = createNode(need_level, key);
  for (char i = need_level; i >= 0; --i) {
    run = need_update[i];
    run->next_nodes[i] = nNode;
  }
  size++;
}

template <typename K, typename V>
bool SkipList<K, V>::Contains(const K& key) {
  SkipListNodePtr run = head;
  SkipListNodePtr next = nullptr;
  for (char i = max_level; i >= 0; --i) {
    while (run) {
      next = run->next_nodes[i];
      if (next == nullptr) break;
      int res = compartor_func(key, next->key);
      if (res > 0)
        run = next;
      else if (res < 0)
        break;
      else
        return true;
    }
  }
  return false;
}

template <typename K, typename V>
void SkipList<K, V>::Clear() {
  if (size == 0) return;
  SkipListNodePtr run = head->next_nodes[0];
  while (run) {
    SkipListNodePtr temp = run->next_nodes[0];
    delete run;
    run = temp;
  }
  for (char i = 0; i < MAX_LEVEL; ++i) head->next_nodes[i] = nullptr;
  size = 0;
  max_level = 0;
}

template <typename K, typename V>
V* SkipList<K, V>::Get(const K& key) {
  SkipListNodePtr run = head;
  SkipListNodePtr next = nullptr;
  for (char i = max_level; i >= 0; --i) {
    while (run) {
      next = run->next_nodes[i];
      if (next == nullptr) break;
      int res = compartor_func(key, next->key);
      if (res > 0)
        run = next;
      else if (res < 0)
        break;
      else
        return &(run->value);
    }
  }
  return nullptr;
}

#endif