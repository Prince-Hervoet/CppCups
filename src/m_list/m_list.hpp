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

 private:
  size_t size;
  mlistMiniNode* head;
  mlistMiniNode* tail;
};
}  // namespace letMeSee