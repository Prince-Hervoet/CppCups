#include <windows.h>

#include <iostream>

#include "num_to_capital.hpp"

int main() {
  system("chcp 65001");
  std::vector<std::string> result = let_me_see::NumToCapital(1000000000LL);
  for (auto it = result.begin(); it != result.end(); ++it) {
    std::cout << (*it);
  }
  return 0;
}