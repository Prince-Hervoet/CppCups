#ifndef _M_SKIPLIST_H_
#define _M_SKIPLIST_H_

#include <random>
#include <vector>

int GetRandomNumber(int min, int max) {
  std::random_device seed;       // 硬件生成随机数种子
  std::ranlux48 engine(seed());  // 利用种子生成随机数引擎
  std::uniform_int_distribution<> distrib(min, max);
  int random = distrib(engine);  // 随机数
  return random;
}

int GetRandomLevel() {
  int level = 0;
  while (GetRandomNumber(1, 101) <= 25) ++level;
  return level;
}

template <typename K, typename T>
class MskiplistNode {
 private:
  K key;
  T value;
  int level;
  MskiplistNode<K, T>** levelPtr;

 public:
  MskiplistNode() {}
  MskiplistNode(K& key, T& value) : key(key), value(value) {}
  MskiplistNode(K& key, T& value, int level)
      : key(key), value(value), level(level) {}

  ~MskiplistNode() {
    if (levelPtr) delete[] levelPtr;
    level = -1;
  }
};

template <typename K, typename T>
class Mskiplist {
  friend class MskiplistNode;
  static const int kDefaultMaxLevel = 31;

 private:
  int current_max_level;
  MskiplistNode<K, T>* header;

 public:
  Mskiplist() {}
  ~Mskiplist() {}

  T* Get(K& key) {
    int startLevel = current_max_level;
    MskiplistNode<K, T>* run = header;
    MskiplistNode<K, T>* next = nullptr;
    for (int i = startLevel; i >= 0; --i) {
      next = run->levelPtr[i];
      while (next != nullptr && next->key < key) {
        run = next;
        next = run->levelPtr[i];
      }
      if (next != nullptr && next->key == key) {
        return &(run->value);
      }
    }
    return nullptr;
  }

  void Insert(K& key, T& value) {
    int level = GetRandomLevel();
    if (level >= Mskiplist::kDefaultMaxLevel)
      level = Mskiplist::kDefaultMaxLevel;
    int startLevel = level > current_max_level ? level : current_max_level;
    MskiplistNode<K, T>* run = header;
    MskiplistNode<K, T>* next = nullptr;
    MskiplistNode<K, T>* nNode = new MskiplistNode<K, T>(key, value, level);
    nNode->levelPtr = new MskiplistNode*[level];
    std::vector<MskiplistNode<K, T>*> needUpdate;
    for (int i = startLevel; i >= 0; --i) {
      next = run->levelPtr[i];
      while (next != nullptr && next->key < key) {
        run = next;
        next = run->levelPtr[i];
      }
      if (next == nullptr || next->key > key) {
        if (i <= level) needUpdate.emplace_back(run);
      } else if (next->key == key) {
        run->value = value;
        delete nNode;
        return;
      }
    }

    for (int i = level; i >= 0; --i) {
      for (int k = 0; k < needUpdate.size(); ++k) {
        MskiplistNode<K, T>* node = needUpdate[k];
        MskiplistNode<K, T>* temp = node->levelPtr[i];
        node->levelPtr[i] = nNode;
        nNode->levelPtr[i] = temp;
      }
    }
    if (current_max_level < level) current_max_level = level;
  }

  void Remove(K& key) {}

 private:
  void initHeader() {
    if (header) return;
    header = new MskiplistNode<K, T>();
    header->level = Mskiplist::kDefaultMaxLevel;
    header->levelPtr = new MskiplistNode*[Mskiplist::kDefaultMaxLevel];
  }
};

#endif  // !1