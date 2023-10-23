#pragma once

namespace let_me_see {

template <typename T>
class SharePtr {
 public:
  SharePtr() : ptr(nullptr), count(nullptr) {}

  SharePtr(T* ptr) : ptr(ptr) { count = new int(1); }

  ~SharePtr() {
    if (!count) return;
    (*count) -= 1;
    if (count == 0 && ptr != nullptr) {
      delete ptr;
      delete count;
    }
    count = nullptr;
    ptr = nullptr;
  }

  SharePtr(const SharePtr& other) {
    if (this == &other) return;
    ptr = other.ptr;
    count = other.count;
    (*count) += 1;
  }

  SharePtr(SharePtr&& other) {
    if (this == &other) return;
    ptr = other.ptr;
    count = other.count;

    other.ptr = nullptr;
    other.count = nullptr;
  }

  T* Get() { return ptr; }

  T* operator->() { return ptr; }

  T& operator*() { return *ptr; }

 private:
  T* ptr;
  int* count;
};
}  // namespace let_me_see