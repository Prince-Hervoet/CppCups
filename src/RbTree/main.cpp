// #include <iostream>
// #include <vector>

// #include "rb_tree.hpp"

#include "rb_tree2.hpp"

int main() {
  RedBlackTree<int, std::string> tree;
  tree.insert(10, "Value1");
  tree.insert(20, "Value2");
  tree.insert(5, "Value3");
  tree.insert(30, "Value4");
  tree.insert(15, "Value5");

  tree.inorderTraversal();

  std::cout << "Search 20: ";
  auto value = tree.search(20);
  if (value != nullptr) {
    std::cout << *value << std::endl;
  } else {
    std::cout << "Not found" << std::endl;
  }

  tree.remove(20);
  tree.inorderTraversal();

  return 0;
}

// int main() {
//   std::vector<int> c;
//   c.at(2);
//   auto func = [](const int& a, const int& b) {
//     if (a > b)
//       return 1;
//     else if (a < b)
//       return -1;
//     else
//       return 0;
//   };
//   RbTree<int, int> rb(func);
//   std::cout << rb.GetSize() << std::endl;
//   rb.Insert(23, 29);
//   rb.Insert(100, 29);
//   rb.Insert(237, 29);
//   rb.Insert(45, 29);
//   rb.Insert(2212, 29);
//   rb.Insert(344, 29);
//   std::cout << rb.GetSize() << std::endl;
//   return 0;
// }