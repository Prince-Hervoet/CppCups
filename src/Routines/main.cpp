#include <cstddef>
#include <iostream>

#include "routines.hpp"
#include "simple_context.hpp"

void test(let_me_see::RoutinesManager *rm, void *args) {
  std::cout << "run test" << std::endl;
}

int main() {
  let_me_see::RoutinesManager rm;
  let_me_see::RoutinePtr routine = rm.CreateRoutine(test, nullptr);
  rm.ResumeRoutine(routine);
  std::cout << "main test" << std::endl;
  //   let_me_see::SimpleContext s1, s2;
  //   s2.rsp = new char[48];
  //   s2.rbp = s2.rsp;
  //   s2.rip = (void *)test;
  //   char *flag = "123123123";
  //   let_me_see::SwapContext(&s1, &s2);
  //   delete[] (char *)(s2.rsp);
  return 0;
}