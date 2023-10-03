#pragma once

namespace letMeSee {
class BitMap {
 public:
  BitMap() : capacity(0) {}
  BitMap(int capacity) : capacity(capacity) {
    size_t ans = bitsToCharCount(capacity);
    bitMapPtr = new char[ans];
  }

  bool HasAt(size_t index);
  void SetZero(size_t index) {}
  int Resize(size_t nSize);
  int Capacity() { return capacity; }

 private:
  int capacity;
  char* bitMapPtr;

  size_t bitsToCharCount(size_t bitCount) {
    size_t ans = (bitCount >> 3);
    if (bitCount % 8 != 0) ans += 1;
    return ans;
  }
};
}  // namespace letMeSee