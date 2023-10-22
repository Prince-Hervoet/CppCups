#pragma once

namespace letMeSee {

template <typename T>
class Mlist {
  struct mlistNode {
    struct mlistNode* next;
    struct mlistNode* prev;
    T data;
  };

  struct mlistMiniNode {
    struct mlistNode* next;
    struct mlistNode* prev;
  };

  using ValueType = T;
  using mlistNode = struct mlistNode;
  using mlistMiniNode = struct mlistMiniNode;

 public:
  Mlist() : size(0) {
    this->head = new mlistMiniNode();
    this->tail = new mlistMiniNode();
    this->head->next = (mlistNode*)(this->tail);
    this->tail->prev = (mlistNode*)(this->head);
    this->endIterator = tail;
  }

  void AddFirst(ValueType& value) {
    mlistNode* node = new mlistNode();
    new (&(node->data)) ValueType(value);
    node->next = head->next;
    head->next = node;
    node->prev = (mlistNode*)head;
    node->next->prev = node;
    size += 1;
  }

  void AddLast(ValueType& value) {
    mlistNode* node = new mlistNode();
    new (&(node->data)) ValueType(value);
    node->prev = tail->prev;
    tail->prev = node;
    node->next = (mlistNode*)tail;
    node->prev->next = node;
    size += 1;
  }

  bool IsEmpty() { return size == 0; }

  size_t Size() { return size; }

  void PopLast() {
    if (size == 0) return;
    mlistNode* target = tail->prev;
    mlistNode* prevNode = target->prev;
    prevNode->next = tail;
    tail->prev = prevNode;
    target->next = nullptr;
    target->prev = nullptr;
    delete target;
    size -= 1;
  }

  void PopFirst() {}

  ValueType& GetLast() {
    if (size == 0) throw "index out of limit";
    return tail->prev.data;
  }

  ValueType& GetFirst() {
    if (size == 0) throw "index out of limit";
    return head->next.data;
  }

  class Iterator {
   public:
    Iterator() {}
    Iterator(Mlist* owner) : owner(owner) { this->nodePtr = owner->head->next; }

    ~Iterator() {}

    bool HasNext() { return nodePtr != owner->tail; }

    ValueType& Next() {
      if (!HasNext()) throw "index out of limit";
      ValueType& value = nodePtr->data;
      nodePtr = nodePtr->next;
      return value;
    }

    void Reset() { nodePtr = owner->head; }

   private:
    Mlist* owner;
    mlistNode* nodePtr;
  };

  Iterator GetIterator() {
    Iterator it(this);
    return it;
  }

  void Remove(Iterator& it) {
    mlistNode* target = it.nodePtr;
    if (target == head || target == tail) return;
    mlistNode* prevNode = target->prev;
    mlistNode* nextNode = target->next;
    prevNode->next = nextNode;
    nextNode->prev = prevNode;
    target->next = nullptr;
    target->prev = nullptr;
    delete target;
    it.nodePtr = nextNode;
    size -= 1;
  }

 private:
  size_t size;
  mlistMiniNode* head;
  mlistMiniNode* tail;
};
}  // namespace letMeSee