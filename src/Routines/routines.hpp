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

// 尝试读取的次数
#define TRY_READ_COUNT 3

// 尝试写入的次数
#define TRY_WRITE_COUNT 3

// 对齐掩码
#define ALIGN_MASK (15ull)

// 获取指针大小
#define ONE_PTR_SIZE sizeof(void*)

// 地址对齐
#define ALIGN_ADDR(x) \
  ((void*)((char*)((uintptr_t)(x) & ~ALIGN_MASK) - ONE_PTR_SIZE))

// 获取数组最终一个元素的地址
#define END_ADDR(array_ptr, size) ((array_ptr) + (size)-1)

class RoutinesManager;
using ArgsType = std::any;
using RoutineTaskType = void (*)(RoutinesManager* rm, ArgsType);

class Routine {
 public:
  using RoutineType = Routine;
  using RoutinePtr = Routine*;
  using RoutinesManagerPtr = RoutinesManager*;

  friend class RoutinesManager;

 private:
  uint64_t routine_id;             // 协程id
  SimpleContext ctx;               // 协程上下文
  char status = INIT_STATUS;       // 协程状态
  char* stack_ptr = nullptr;       // 协程栈空间指针
  unsigned int stack_size = 0;     // 栈大小
  unsigned int used_size = 0;      // 已经使用的大小，暂时没有使用
  RoutineTaskType func = nullptr;  // 协程执行的函数
  ArgsType args = nullptr;         // 协程执行的函数的参数
  RoutinePtr parent = nullptr;     // 该协程的父协程

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
  EpollPack ep;                  // epoll 结构体
  RoutinePtr current = nullptr;  // 当前正在执行的routine
  SimpleContext host;            // 主上下文
  static uint64_t inc_id;        // 递增routine id

 public:
  RoutinesManager() { initEpollPack(); }
  ~RoutinesManager() {}

 public:
  /**
    创建协程结构
  */
  static RoutinePtr CreateRoutine(RoutineTaskType func, void* args,
                                  unsigned int size = SINGLE_STACK_SIZE);

  /**
    执行协程
  */
  void ResumeRoutine(RoutinePtr routine);

  /**
    让出协程
  */
  void YieldRoutine();

  /**
    释放协程
  */
  void CloseRoutine(RoutinePtr routine);

  /**
    获取当前协程结构
  */
  RoutinePtr GetCurrent() const { return current; }
  ssize_t EpollRoutineRead(int fd, char* buffer, size_t size);
  ssize_t EpollRoutineWrite(int fd, char* data, size_t size);
  EpollPackType& GetEpollPack() { return ep; }

 private:
  /**
   初始化协程属性
  */
  void initRoutine(RoutinePtr routine);

  /**
    内部协程执行的函数，会调用用户传入的函数
  */
  static void innerRoutineRun(RoutinesManagerPtr rm);

  void initEpollPack() {
    if (!ep.GetIsClosed()) return;
    ep.EpollCreate();
  }
};

using RoutineType = Routine;
using RoutinePtr = Routine*;
using RoutinesManagerPtr = RoutinesManager*;
}  // namespace let_me_see

#endif