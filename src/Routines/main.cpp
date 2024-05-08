#include <ucontext.h>

#include <cstdlib>
#include <iostream>

#include "routines.hpp"

void test(let_me_see::RoutinesManagerPtr rm, void *args) {
  int a = 1;
  rm->YieldRoutine();
  std::cout << "run test" << std::endl;
  int b = 10;
}

int main() {
  // let_me_see::SimpleContext c0, c1;
  // c1.rsp = let_me_see::AlignAddress((char *)malloc(877) + 876);
  // c1.rbp = c1.rsp;
  // c1.rip = (void *)test;
  // let_me_see::SetContext(&c1);
  // let_me_see::SwapContext(&c0, &c1);
  // free((char *)c1.rsp);
  let_me_see::RoutinesManager rm;
  let_me_see::RoutinePtr routine =
      let_me_see::RoutinesManager::CreateRoutine(test, nullptr);

  rm.ResumeRoutine(routine);
  rm.ResumeRoutine(routine);
  std::cout << "run main" << std::endl;
  return 0;
}