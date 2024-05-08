#include "routines.hpp"

#include <cassert>
#include <cstring>
#include <exception>
#include <iostream>

#include "simple_context.hpp"

#define MAKE_MEMORY_FLAG char flag = 0
#define MEMORY_FLAG_NAME flag

namespace let_me_see {

// void RoutinesManager::moveRoutineOut(RoutinePtr routine, char* current_top) {
//   if (routine == nullptr) return;
//   char* max_top = share_stack + SHARE_STACK_SIZE;
//   unsigned int used_size = max_top - current_top;
//   assert(used_size <= SHARE_STACK_SIZE);
//   if (used_size > routine->stack_size) {
//     unsigned int new_size =
//         static_cast<unsigned int>(used_size * EXPAND_FACTOR);
//     if (routine->stack_ptr) delete[] routine->stack_ptr;
//     routine->stack_ptr = new char[new_size];
//     routine->stack_size = new_size;
//   }
//   routine->used_size = used_size;
//   std::memcpy(routine->stack_ptr, current_top, used_size);
// }

// void RoutinesManager::moveRoutineIn(RoutinePtr routine) {
//   if (routine == nullptr) return;
//   char* dest_ptr = fix_stack_ptr - routine->used_size + 1;
//   std::memcpy(dest_ptr, routine->stack_ptr, routine->used_size);
// }

void RoutinesManager::initRoutine(RoutinePtr routine) {
  if (routine->status != INIT_STATUS) return;
  SimpleContext* ctx_ptr = &(routine->ctx);
  char* ptr = new char[SINGLE_STACK_SIZE];
  ctx_ptr->rsp = ALIGN_ADDRESS((ptr + SINGLE_STACK_SIZE - 1));
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
  routine->used_size = 0;
  routine->stack_ptr = nullptr;
  routine->stack_size = 0;
  RoutinePtr ret = routine->parent;
  routine->parent = nullptr;
  rm->current = ret;
  SimpleContextPtr dest_ctx = ret == nullptr ? &(rm->host) : &(ret->ctx);
  if (ret) ret->status = RUNNING_STATUS;
  SetContext(dest_ctx);
}

void RoutinesManager::ResumeRoutine(RoutinePtr routine) {
  if (routine == nullptr) return;
  if (routine->status & (DEAD_STATUS | RUNNING_STATUS)) return;
  if (routine->status == INIT_STATUS) initRoutine(routine);
  SimpleContextPtr src_ctx = &host;
  if (current) {
    src_ctx = &(current->ctx);
    current->status = SUSPEND_STATUS;
  }
  routine->parent = current;
  current = routine;
  current->status = RUNNING_STATUS;
  SwapContext(src_ctx, &(current->ctx));
}

void RoutinesManager::YieldRoutine() {
  if (current == nullptr) return;
  SimpleContextPtr src_ctx = &(current->ctx);
  SimpleContextPtr dest_ctx = &host;
  RoutinePtr ret = current->parent;
  if (ret) {
    dest_ctx = &(current->parent->ctx);
    current->parent->status = RUNNING_STATUS;
  }
  current->status = SUSPEND_STATUS;
  current->parent = nullptr;
  current = ret;
  SwapContext(src_ctx, dest_ctx);
}

}  // namespace let_me_see