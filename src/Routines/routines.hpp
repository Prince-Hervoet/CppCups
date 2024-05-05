#ifndef _ROUTINES_H_
#define _ROUTINES_H_

#include <cstddef>
#include <set>

#include "simple_context.hpp"

#define INIT_STATUS 0
#define READY_STATUS 1
#define SUSPEND_STATUS 2
#define RUNNING_STATUS 4
#define DEAD_STATUS 8
#define SHARE_STACK_SIZE 65536

namespace let_me_see {

using ContextFunc = void (*)();

class RoutinesManager;
using TaskType = void (*)(RoutinesManager* rm, void*);
class Routine {
  friend class RoutinesManager;
  using RoutineType = Routine;
  using RoutinePtr = Routine*;
  using RoutinesManagerPtr = RoutinesManager*;
  using ArgsType = void*;

 private:
  SimpleContext ctx;
  char status = INIT_STATUS;
  char* stack_ptr = nullptr;
  unsigned int stack_size = 0;
  TaskType func = nullptr;
  ArgsType args = nullptr;
  RoutinePtr parent = nullptr;
};

class RoutinesManager {
  using RoutineType = Routine;
  using RoutinePtr = Routine*;
  using RoutinesManagerPtr = RoutinesManager*;

 private:
  char share_stack[SHARE_STACK_SIZE];
  std::set<RoutineType> routine_list;
  RoutinePtr current = nullptr;
  SimpleContext host;

 public:
  void Start(RoutinePtr routine);
  RoutinePtr CreateRoutine(TaskType func, void* args);
  void ResumeRoutine(RoutinePtr routine);
  static void YieldRoutine(RoutinesManagerPtr rm);

 private:
  void initRoutine(RoutinePtr routine);
  static void innerRoutineRun(RoutinesManagerPtr rm, void* args);
};

using RoutineType = Routine;
using RoutinePtr = Routine*;
using RoutinesManagerPtr = RoutinesManager*;
}  // namespace let_me_see

#endif