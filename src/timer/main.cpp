#include <iostream>

#include "timer_manager.hpp"

void test1(void* args) { std::cout << "test1" << std::endl; }
void test2(void* args) { std::cout << "test2" << std::endl; }

int main() {
  TimerManager tm;
  std::function<void(void*)> func1(test1);
  std::function<void(void*)> func2(test2);
  tm.Register(func1, 1000, 10);
  tm.Register(func2, 2000, 2);

  while (tm.GetSize() > 0) {
    tm.updateTask();
  }
  return 0;
}