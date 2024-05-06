#include "routines.hpp"

#include <cassert>
#include <cstring>
#include <exception>
#include <iostream>

#include "simple_context.hpp"

#define MAKE_MEMORY_FLAG char flag = 0
#define MEMORY_FLAG_NAME flag

namespace let_me_see {

void RoutinesManager::moveRoutineSpace(RoutinePtr routine, char* current_top) {
  if (routine == nullptr) return;
  char* max_top = share_stack + SHARE_STACK_SIZE;
  unsigned int used_size = max_top - current_top;
  assert(used_size <= SHARE_STACK_SIZE);
  if (used_size > routine->stack_size) {
    delete[] routine->stack_ptr;
    routine->stack_ptr = new char[used_size];
    routine->stack_size = used_size;
  }
  memcpy(routine->stack_ptr, share_stack, used_size);
  memset(share_stack, 0, SHARE_STACK_SIZE);
}

void RoutinesManager::initRoutine(RoutinePtr routine) {
  if (routine->status != INIT_STATUS) return;
  SimpleContext* ctx_ptr = &(routine->ctx);
  ctx_ptr->rsp = share_stack + SHARE_STACK_SIZE - 1;
  ctx_ptr->rbp = ctx_ptr->rsp;
  ctx_ptr->rip = (void*)innerRoutineRun;
  ctx_ptr->rcx = this;
  ctx_ptr->rdx = routine->args;
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
  RoutinePtr routine = rm->current;
  if (routine) {
    try {
      routine->func(rm, routine->args);
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
  if (routine) {
    routine->status = DEAD_STATUS;
    delete[] routine->stack_ptr;
    routine->stack_size = 0;
    RoutinePtr ret = routine->parent;
    SimpleContextPtr ctx_ptr = ret == nullptr ? &(rm->host) : &(ret->ctx);
    routine->parent = nullptr;
    rm->current = nullptr;
    SetContext(ctx_ptr);
  }
}

void RoutinesManager::ResumeRoutine(RoutinePtr routine) {
  MAKE_MEMORY_FLAG;
  if (routine == nullptr) return;
  if ((routine->status & (DEAD_STATUS | RUNNING_STATUS))) return;
  moveRoutineSpace(current, &MEMORY_FLAG_NAME);
  initRoutine(routine);
  SimpleContext* src_ctx = &host;
  if (current) {
    src_ctx = &(current->ctx);
    current->status = SUSPEND_STATUS;
    routine->parent = current;
  }
  routine->status = RUNNING_STATUS;
  current = routine;
  char* dd = "123123123";
  SwapContext(src_ctx, &(routine->ctx));
}

}  // namespace let_me_see