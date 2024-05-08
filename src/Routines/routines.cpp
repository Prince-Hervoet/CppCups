#include "routines.hpp"

#include <cassert>
#include <cstring>
#include <exception>
#include <iostream>

#include "simple_context.hpp"

#define MAKE_MEMORY_FLAG char flag = 0
#define MEMORY_FLAG_NAME flag

namespace let_me_see {

void* AlignAddress(void* ptr) {
  return (char*)((uintptr_t)(ptr) & ~15ull) - sizeof(void*);
}

void RoutinesManager::moveRoutineOut(RoutinePtr routine, char* current_top) {
  if (routine == nullptr) return;
  unsigned int used_size = share_stack + SHARE_STACK_SIZE - current_top;
  assert(used_size <= SHARE_STACK_SIZE);
  if (used_size > routine->stack_size) {
    unsigned int new_size = static_cast<unsigned int>(used_size * 1.5);
    delete[] routine->stack_ptr;
    routine->stack_ptr = new char[new_size];
    routine->stack_size = new_size;
  }
  routine->used_size = used_size;
  memcpy(routine->stack_ptr, current_top, used_size);
  // memset(share_stack, 0, SHARE_STACK_SIZE);
}

void RoutinesManager::moveRoutineIn(RoutinePtr routine) {
  if (routine == nullptr) return;
  char* dest_ptr = fix_stack_ptr - routine->used_size + 1;
  memcpy(dest_ptr, routine->stack_ptr, routine->used_size);
}

void RoutinesManager::initRoutine(RoutinePtr routine) {
  if (routine->status != INIT_STATUS) return;
  SimpleContext* ctx_ptr = &(routine->ctx);
  ctx_ptr->rsp = fix_stack_ptr;
  ctx_ptr->rbp = ctx_ptr->rsp;
  ctx_ptr->rip = (void*)innerRoutineRun;
  ctx_ptr->rdi = this;
  routine->status = READY_STATUS;
}

RoutinePtr RoutinesManager::CreateRoutine(TaskType func, void* args) {
  if (func == nullptr) return nullptr;
  RoutinePtr nRoutine = new Routine();
  nRoutine->status = INIT_STATUS;
  nRoutine->func = func;
  nRoutine->args = args;
  return nRoutine;
}

void RoutinesManager::innerRoutineRun(RoutinesManagerPtr rm) {
  assert(rm != nullptr);
  RoutinePtr routine = rm->current;
  assert(routine != nullptr);
  try {
    routine->func(rm, routine->args);
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  routine->status = DEAD_STATUS;
  delete[] routine->stack_ptr;
  routine->stack_size = 0;
  RoutinePtr ret = routine->parent;
  SimpleContextPtr ctx_ptr = ret == nullptr ? &(rm->host) : &(ret->ctx);
  routine->parent = nullptr;
  rm->current = nullptr;
  SetContext(ctx_ptr);
}

void RoutinesManager::ResumeRoutine(RoutinePtr routine) {
  MAKE_MEMORY_FLAG;  // 内存标记，此时这个变量在栈顶
  if (routine == nullptr) return;
  // 如果当前协程是死亡或者正在执行的状态就不继续
  if ((routine->status & (DEAD_STATUS | RUNNING_STATUS))) return;
  // 先尝试将当前正在执行的协程copy出去（如果有的话）
  moveRoutineOut(current, &MEMORY_FLAG_NAME);
  if (routine->status == INIT_STATUS)
    initRoutine(routine);
  else if (routine->status == SUSPEND_STATUS)
    moveRoutineIn(routine);
  SimpleContext* src_ctx = &host;
  if (current) {
    src_ctx = &(current->ctx);
    current->status = SUSPEND_STATUS;
    routine->parent = current;
  }
  routine->status = RUNNING_STATUS;
  current = routine;
  SwapContext(src_ctx, &(routine->ctx));
}

void RoutinesManager::YieldRoutine() {
  MAKE_MEMORY_FLAG;  // 内存标记，此时这个变量在栈顶
  if (current == nullptr) return;
  // 先尝试将当前正在执行的协程copy出去（如果有的话）
  moveRoutineOut(current, &MEMORY_FLAG_NAME);
  SimpleContextPtr desc_ctx = &host;
  if (current->parent) desc_ctx = &(current->parent->ctx);
  current->status = SUSPEND_STATUS;
  current = current->parent;
  moveRoutineIn(current);
  SwapContext(&(current->ctx), desc_ctx);
}

}  // namespace let_me_see