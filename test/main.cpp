#include <cstddef>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_set>

static size_t getIndex(void* key, int n) {
  size_t ptrValue = (size_t)(key);
  ptrValue ^= (ptrValue >> 16);
  return ptrValue & (n - 1);
}

int main() {
  size_t value = std::hash<int>{}(212);
  std::cout << value << std::endl;
}