#include <cstdio>
#include <iostream>

#include "skip_list.hpp"

int main() {
  SkipList<int, int> sl([](const int& a, const int& b) {
    if (a > b)
      return 1;
    else if (a < b)
      return -1;
    return 0;
  });
  int a = 199;
  sl.Put(a);
  std::cout << sl.Contains(a) << std::endl;

  for (int i = 0; i < 20; i++) {
    printf("%d", getRandomLevel());
  }
  return 0;
}