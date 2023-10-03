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
  void Stop();

 private:
  int coreSize;
  int coreCapacity;
  int threadSize;
  int threadCapacity;
  int taskCapacity;
  bool isActive;
  std::mutex mu;
  std::condition_variable cond;
  std::condition_variable addCond;
  std::queue<WorkerTask*> tasks;
  std::set<Worker*> threads;

  int addTask(WorkerTask* task);
  bool createThread(bool isCore, WorkerTask* task);
  bool addTaskToQueue(WorkerTask* task, bool isTry);
  void removeWorker(Worker* w);
};
}  // namespace letMeSee