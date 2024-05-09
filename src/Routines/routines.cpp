#include "routines.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <exception>
#include <iostream>

#include "simple_context.hpp"


#define MAKE_MEMORY_FLAG char flag = 0
#define MEMORY_FLAG_NAME flag

namespace let_me_see {

uint64_t RoutinesManager::inc_id = 0xA;

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

void RoutinesManager::initEpollPack() {
  if (!ep.GetIsClosed()) return;
  ep.EpollCreate();
}

ssize_t RoutinesManager::EpollRoutineRead(int fd, char* buffer, size_t size) {
  int res = 0, empty_count = 0;
  do {
    if (empty_count >= TRY_READ_COUNT) {
      epoll_data_t data;
      data.ptr = current;
      EpollEventType ev = EpollPack::MakeEvent(EPOLLIN, data);
      ep.EpollCtl(EPOLL_CTL_ADD, fd, &ev);
      YieldRoutine();
    }
    empty_count++;
    res = NonBlockRead(fd, buffer, size);
  } while (res == 0);
  return res;
}

ssize_t RoutinesManager::EpollRoutineWrite(int fd, char* data, size_t size) {
  int res = 0, empty_count = 0;
  do {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      if (empty_count == TRY_READ_COUNT) {
        epoll_data_t data;
        data.ptr = current;
        EpollEventType ev = EpollPack::MakeEvent(EPOLLOUT, data);
        ep.EpollCtl(EPOLL_CTL_ADD, fd, &ev);
        YieldRoutine();
      }
    } else {
      break;
    }
    empty_count++;
    res = NonBlockWrite(fd, data, size);
  } while (res < 0);
  return res;
}

void RoutinesManager::initRoutine(RoutinePtr routine) {
  if ((routine->status & INIT_STATUS) != 0) return;
  SimpleContext* ctx_ptr = &(routine->ctx);
  routine->stack_ptr = new char[routine->stack_size];
  ctx_ptr->rsp = ALIGN_ADDR(END_ADDR(routine->stack_ptr, routine->stack_size));
  ctx_ptr->rbp = ctx_ptr->rsp;
  ctx_ptr->rip = (void*)innerRoutineRun;
  ctx_ptr->rdi = this;
  routine->setStatus(READY_STATUS);
}

RoutinePtr RoutinesManager::CreateRoutine(TaskType func, void* args,
                                          unsigned int size) {
  if (func == nullptr) return nullptr;
  RoutinePtr nRoutine = new Routine();
  nRoutine->status = INIT_STATUS;
  nRoutine->func = func;
  nRoutine->args = args;
  nRoutine->stack_size = size;
  nRoutine->parent = nullptr;
  nRoutine->routine_id = RoutinesManager::inc_id++;
  return nRoutine;
}

void RoutinesManager::innerRoutineRun(RoutinesManagerPtr rm) {
  assert(rm != nullptr);
  RoutinePtr routine = rm->current;
  assert(routine != nullptr);
  try {
    routine->func(rm, routine->args);
  } catch (std::exception& e) {
    std::cout << "id: " << routine->routine_id << " " << e.what() << std::endl;
  }
  routine->status = DEAD_STATUS;
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

void RoutinesManager::CloseRoutine(RoutinePtr routine) {
  if (routine == nullptr) return;
  if (routine->status & RUNNING_STATUS) return;
  if (routine->stack_ptr) delete[](routine->stack_ptr);
  routine->stack_size = 0;
  delete routine;
}

}  // namespace let_me_see