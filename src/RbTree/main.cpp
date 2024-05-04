#include <iostream>
#include <vector>

#include "rb_tree.hpp"

int main() {
  std::vector<int> c;
  c.at(2);
  auto func = [](const int& a, const int& b) {
    if (a > b)
      return 1;
    else if (a < b)
      return -1;
    else
      return 0;
  };
  RbTree<int, int> rb(func);
  std::cout << rb.GetSize() << std::endl;
  rb.Insert(23, 29);
  rb.Insert(100, 29);
  rb.Insert(237, 29);
  rb.Insert(45, 29);
  rb.Insert(2212, 29);
  rb.Insert(344, 29);
  std::cout << rb.GetSize() << std::endl;
  return 0;
}