#pragma once

namespace let_me_see {
template <typename T>
class UniquePtr {
 public:
  UniquePtr() {}
  UniquePtr(T* ptr) : ptr(ptr) {}

  ~UniquePtr() {
    if (ptr) delete ptr;
  }

  UniquePtr(const UniquePtr& other) = delete;
  UniquePtr(UniquePtr&& other) {
    if (this == &other) return;
    ptr = other.ptr;
    other.ptr = nullptr;
  }

  T* Get() { return ptr; }

  T* operator->() { return ptr; }

  T& operator*() { return *ptr; }

 private:
  T* ptr;
};
}  // namespace let_me_see