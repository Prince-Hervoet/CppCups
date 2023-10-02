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
  Worker(ThreadPool* owner, WorkerTask* task, bool isCore)
      : owner(owner), firstTask(task), isCore(isCore) {}

  void Start() {
    if (isActive) return;
    isActive = true;
    std::thread nThread(threadRunFunc, this);
    this->t = std::move(nThread);
  }

  void Stop() {
    if (!isActive) return;
    isActive = false;
  }

  void Join() {
    if (t.joinable()) t.join();
  }

  void Detach() {
    if (t.joinable()) t.detach();
  }

  bool IsActive() { return isActive; }

 private:
  volatile bool isActive;
  bool isCore;
  ThreadPool* owner;
  std::thread t;
  WorkerTask* firstTask;
  static void threadRunFunc(void* args);
  static WorkerTask* getTaskFromQueue(Worker* w);
};
}  // namespace letMeSee