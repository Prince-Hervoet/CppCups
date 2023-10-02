#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <set>

#include "worker.hpp"

namespace letMeSee {
class ThreadPool {
  friend class Worker;

 public:
  using TaskType = void (*)(void*);

 private:
  typedef struct taskBlock {
    TaskType taskFunc;
    void* args;
  } taskBlock;

 public:
  int Commit(TaskType task, void* args);
  int CoreSize() { return coreSize; }
  int Size() { return threadSize; }
  int CoreCapacity() { return coreCapacity; }
  int Capacity() { return threadCapacity; }

 private:
  int coreSize;
  int coreCapacity;
  int threadSize;
  int threadCapacity;
  bool isActive;
  std::mutex mu;
  std::condition_variable cond;
  std::queue<WorkerTask*> tasks;
  std::set<std::thread> threads;

  static void threadRunFunc(void* args);
  int addTask(taskBlock* task);
  bool createThread(bool isCore, taskBlock* task);
  bool addTaskToQueue(taskBlock* task, bool isTry);
};
}  // namespace letMeSee