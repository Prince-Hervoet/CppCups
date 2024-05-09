#ifndef _ROUTINES_H_
#define _ROUTINES_H_

#include <any>
#include <cstddef>
#include <cstdint>

#include "epoll_pack.hpp"
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
#define TRY_READ_COUNT 3
#define ALIGN_MASK (15ull)
#define ONE_PTR_SIZE sizeof(void*)
#define ALIGN_ADDR(x) \
  ((void*)((char*)((uintptr_t)(x) & ~ALIGN_MASK) - ONE_PTR_SIZE))

#define END_ADDR(array_ptr, size) ((array_ptr) + (size)-1)

class RoutinesManager;
using ArgsType = std::any;
using TaskType = void (*)(RoutinesManager* rm, ArgsType);

class Routine {
 public:
  using RoutineType = Routine;
  using RoutinePtr = Routine*;
  using RoutinesManagerPtr = RoutinesManager*;

  friend class RoutinesManager;

 private:
  uint64_t routine_id;
  SimpleContext ctx;
  char status = INIT_STATUS;
  char* stack_ptr = nullptr;
  unsigned int stack_size = 0;
  unsigned int used_size = 0;
  TaskType func = nullptr;
  ArgsType args = nullptr;
  RoutinePtr parent = nullptr;

 public:
  RoutinePtr GetParent() const { return parent; }
  unsigned int GetStackSize() const { return stack_size; }
  char GetStatus() const { return status; }
  uint64_t GetRoutineId() const { return routine_id; }

 private:
  void setStatus(char newStatus) { status = newStatus; }
};

class RoutinesManager {
 public:
  using RoutineType = Routine;
  using RoutinePtr = Routine*;
  using RoutinesManagerPtr = RoutinesManager*;

 private:
  EpollPack ep;
  RoutinePtr current = nullptr;
  SimpleContext host;
  static uint64_t inc_id;

 public:
  RoutinesManager() { initEpollPack(); }
  ~RoutinesManager() {}

 public:
  static RoutinePtr CreateRoutine(TaskType func, void* args,
                                  unsigned int size = SINGLE_STACK_SIZE);
  void ResumeRoutine(RoutinePtr routine);
  void YieldRoutine();
  void CloseRoutine(RoutinePtr routine);
  RoutinePtr GetCurrent() const { return current; }
  ssize_t EpollRoutineRead(int fd, char* buffer, size_t size);
  ssize_t EpollRoutineWrite(int fd, char* data, size_t size);

 private:
  void initRoutine(RoutinePtr routine);
  void initEpollPack();
  static void innerRoutineRun(RoutinesManagerPtr rm);
};

using RoutineType = Routine;
using RoutinePtr = Routine*;
using RoutinesManagerPtr = RoutinesManager*;
}  // namespace let_me_see

#endif