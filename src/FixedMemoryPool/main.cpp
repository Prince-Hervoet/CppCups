#include <iostream>

#include "fixed_memory_pool.hpp"

int main() {
  let_me_see::FixedMemoryPool pool(40);
  void* ptr = pool.GetMemory();
  std::cout << ptr << std::endl;
  return 0;
}