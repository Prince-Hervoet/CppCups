#pragma once
#include <iostream>
#include <string>

namespace letMeSee {

const size_t REALLOC_FLAG = 1024;

template <typename T>
class Mvector {
  using ValueType = T;

 public:
  Mvector() : size(0), capacity(0), dataPtr(nullptr) {}

  Mvector(size_t capacity) : size(0), capacity(capacity) {
    this->dataPtr = new ValueType[capacity];
  }

  Mvector(size_t size, size_t capacity) : size(size), capacity(capacity) {
    this->dataPtr = new ValueType[capacity];
    memset(dataPtr, 0, capacity * sizeof(ValueType));
  }

  ~Mvector() {
    if (dataPtr) delete[] dataPtr;
  }

  Mvector(const Mvector& other) { copyTo(other, *this); }

  Mvector(Mvector&& other) {
    if (this == &other) return;
    this->dataPtr = other.dataPtr;
    this->capacity = other.capacity;
    this->size = other.size;
    other.dataPtr = nullptr;
    other.size = 0;
    other.capacity = 0;
  }

  void PushBack(ValueType& value) { this->EmplaceBack(value); }

  void PushBack(ValueType&& value) { this->EmplaceBack(std::move(value)); }

  template <typename... Args>
  void EmplaceBack(Args&&... args) {
    if (size == capacity) {
      size_t nCapacity = size + 1;
      if (capacity < REALLOC_FLAG) {
        nCapacity = (nCapacity << 1);
      } else {
        nCapacity = (nCapacity | (size >> 1));
      }
      realloc(nCapacity);
    }
    new (&dataPtr[size]) ValueType(std::forward<Args>(args)...);
    size += 1;
  }

  size_t Size() { return size; }

  size_t Capacity() { return capacity; }

  ValueType& Get(size_t index) {
    if (index < 0 || index >= size) throw "index out of limit";
    return dataPtr[index];
  }

  void PopBack() {
    if (size == 0) return;
    size_t nSize = size - 1;
    this->dataPtr[nSize].~ValueType();
    size = nSize;
  }

  bool IsEmpty() { return size == 0; }

  void Clear() {
    if (size == 0) return;
    for (size_t i = 0; i < size; ++i) {
      dataPtr[i].~ValueType();
    }
    size = 0;
  }

  void Resize(size_t nSize) {
    if (nSize == capacity) {
      memset(dataPtr, 0, capacity * sizeof(ValueType));
      return;
    }
    size_t nCapacity = nSize;
    ValueType* nDataPtr = new ValueType[nCapacity];
    size = nCapacity;
    capacity = nCapacity;
    delete[] dataPtr;
    dataPtr = nDataPtr;
    memset(dataPtr, 0, capacity * sizeof(ValueType));
  }

  ValueType& operator[](int index) {
    if (index < 0 || index >= size) throw "index out of limit";
    return dataPtr[index];
  }

  Mvector& operator=(Mvector& other) {
    if (this == &other) return *this;
    copyTo(other, *this);
  }

 private:
  size_t size;
  size_t capacity;
  ValueType* dataPtr;

  void realloc(size_t nCapacity) {
    ValueType* nDataPtr = new ValueType[nCapacity];
    for (size_t i = 0; i < size; ++i) {
      new (&nDataPtr[i]) ValueType(std::move(dataPtr[i]));
    }
    delete[] dataPtr;
    dataPtr = nDataPtr;
    capacity = nCapacity;
  }

  void copyTo(Mvector& source, Mvector& target) {
    if (target.dataPtr) {
      delete[] target.dataPtr;
      target.capacity = 0;
      target.size = 0;
    }
    target.dataPtr = new ValueType[source.capacity];
    try {
      for (size_t i = 0; i < source.size; ++i) {
        new (&(target.dataPtr[i])) ValueType(source.dataPtr[i]);
        target.size += 1;
      }
    } catch (std::exception& e) {
      for (size_t i = 0; i < target.size; ++i) {
        target.dataPtr[i].~ValueType();
      }
      delete[] target.dataPtr;
      std::cout << e.what() << std::endl;
      throw "copy error";
    }
    target.size = source.size;
    target.capacity = source.capacity;
  }
};
}  // namespace letMeSee