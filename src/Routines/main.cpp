#include <ucontext.h>

#include <cstdlib>
#include <iostream>

#include "routines.hpp"

void test(let_me_see::RoutinesManagerPtr rm, let_me_see::ArgsType args) {
  let_me_see::RoutinePtr temp = rm->GetCurrent();
  int a = 1;
  std::cout << temp->GetParent() << std::endl;
  rm->YieldRoutine();
  std::cout << "run test" << std::endl;
  std::cout << temp->GetParent() << std::endl;
  int b = 10;
}

int main() {
  let_me_see::RoutinesManager rm;
  let_me_see::RoutinePtr routine =
      let_me_see::RoutinesManager::CreateRoutine(test, nullptr, 2048);
  std::cout << "1: " << routine->GetParent() << std::endl;
  rm.ResumeRoutine(routine);
  std::cout << "2: " << routine->GetParent() << std::endl;
  rm.ResumeRoutine(routine);
  std::cout << "3: " << routine->GetParent() << std::endl;
  rm.CloseRoutine(routine);
  std::cout << "run main" << std::endl;
  return 0;
}