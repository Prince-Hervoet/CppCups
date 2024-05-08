#ifndef _ROUTINES_H_
#define _ROUTINES_H_

#include <cstdint>
#include <set>

#include "simple_context.hpp"

#define INIT_STATUS 0
#define READY_STATUS 1
#define SUSPEND_STATUS 2
#define RUNNING_STATUS 4
#define DEAD_STATUS 8
#define SHARE_STACK_SIZE 65536
#define SINGLE_STACK_SIZE 65536

namespace let_me_see {

#define EXPAND_FACTOR (1.5)
#define ALIGN_MASK (15ull)
#define ONE_PTR_SIZE sizeof(void*)
#define ALIGN_ADDRESS(x) \
  ((void*)((char*)((uintptr_t)(x) & ~ALIGN_MASK) - ONE_PTR_SIZE))

using ContextFunc = void (*)();
class RoutinesManager;
using ArgsType = void*;
using TaskType = void (*)(RoutinesManager* rm, void*);

class Routine {
 public:
  using RoutineType = Routine;
  using RoutinePtr = Routine*;
  using RoutinesManagerPtr = RoutinesManager*;

  friend class RoutinesManager;

 private:
  SimpleContext ctx;
  char status = INIT_STATUS;
  char* stack_ptr = nullptr;
  unsigned int stack_size = 0;
  unsigned int used_size = 0;
  TaskType func = nullptr;
  ArgsType args = nullptr;
  RoutinePtr parent = nullptr;
};

class RoutinesManager {
 public:
  using RoutineType = Routine;
  using RoutinePtr = Routine*;
  using RoutinesManagerPtr = RoutinesManager*;

 private:
  std::set<RoutinePtr> routine_list;
  RoutinePtr current = nullptr;
  SimpleContext host;

 public:
  RoutinesManager() {
    // share_stack = new char[SHARE_STACK_SIZE];
    // fix_stack_ptr =
    //     (char*)ALIGN_ADDRESS((char*)share_stack + SHARE_STACK_SIZE - 1);
  }

  ~RoutinesManager() {}

 public:
  static RoutinePtr CreateRoutine(TaskType func, void* args);
  void ResumeRoutine(RoutinePtr routine);
  void YieldRoutine();

  void JoinList(RoutinePtr routine) {
    if (routine == nullptr) return;
    routine_list.insert(routine);
  }

  int GetListSize() const { return routine_list.size(); }

  void StartList() {
    auto end = routine_list.end();
    for (auto it = routine_list.begin(); it != end; ++it) {
      RoutinePtr ptr = *it;
      ResumeRoutine(ptr);
    }
  }

 private:
  void initRoutine(RoutinePtr routine);
  // void moveRoutineOut(RoutinePtr routine, char* current_top);
  // void moveRoutineIn(RoutinePtr routine);
  static void innerRoutineRun(RoutinesManagerPtr rm);
};

using RoutineType = Routine;
using RoutinePtr = Routine*;
using RoutinesManagerPtr = RoutinesManager*;
void* AlignAddress(void* ptr);
}  // namespace let_me_see

#endif