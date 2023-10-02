#pragma once

#include <thread>

namespace letMeSee {

using TaskType = void (*)(void*);

typedef struct WorkerTask {
  TaskType taskFunc;
  void* args;
} WorkerTask;

class ThreadPool;
class Worker {
 public:
  Worker() {}
  Worker(ThreadPool* owner) : owner(owner) {}
  Worker(ThreadPool* owner, WorkerTask* task) : owner(owner), firstTask(task) {}

  void Start() {
    if (isActive) return;
    isActive = true;
    std::thread nThread(threadRunFunc, this);
    this->t = std::move(nThread);
  }

 private:
  volatile bool isActive;
  ThreadPool* owner;
  std::thread t;
  WorkerTask* firstTask;
  static void threadRunFunc(void* args);
  static WorkerTask* getTaskFromQueue(Worker* w);
};
}  // namespace letMeSee