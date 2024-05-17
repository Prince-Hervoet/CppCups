#include <iostream>

#include "bit_map.hpp"

int main() {
  let_me_see::BitMap bitmap(20);
  bitmap.Set(22);
  std::cout << bitmap.Test(22) << std::endl;
  return 0;
}