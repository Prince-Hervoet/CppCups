#include "routines.hpp"

#include <cassert>

#include "simple_context.hpp"

#define MAKE_MEMORY_FLAG char flag = 0
#define MEMORY_FLAG_NAME flag

namespace let_me_see {

static void moveRoutineSpace(RoutinePtr routine, char* array_last,
                             char* current_top) {
  if (routine == nullptr) return;
  unsigned int used_size = array_last - current_top;
  assert(used_size <= SHARE_STACK_SIZE);
  return;
}

void RoutinesManager::initRoutine(RoutinePtr routine) {
  if (routine->status != INIT_STATUS) return;
  SimpleContext* ctxPtr = &(routine->ctx);
  ctxPtr->rsp = share_stack + SHARE_STACK_SIZE - 1;
  ctxPtr->rbp = ctxPtr->rsp;
  ctxPtr->rip = (void*)innerRoutineRun;
  ctxPtr->rcx = this;
  ctxPtr->rdx = routine->args;
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

void RoutinesManager::innerRoutineRun(RoutinesManagerPtr rm, void* args) {}

void RoutinesManager::ResumeRoutine(RoutinePtr routine) {
  MAKE_MEMORY_FLAG;
  if (routine == nullptr) return;
  if ((routine->status & (DEAD_STATUS | RUNNING_STATUS))) return;
  moveRoutineSpace(current, share_stack + SHARE_STACK_SIZE, &MEMORY_FLAG_NAME);
  initRoutine(routine);
  SimpleContext* src_ctx = &host;
  if (current) {
    src_ctx = &(current->ctx);
    current->status = SUSPEND_STATUS;
    routine->parent = current;
    current = routine;
  }
  SwapContext(src_ctx, &(routine->ctx));
}

}  // namespace let_me_see