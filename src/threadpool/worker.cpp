#include "worker.hpp"

#include "threadpool.hpp"

namespace letMeSee {
void Worker::threadRunFunc(void* args) {
  Worker* w = (Worker*)args;
  WorkerTask* task = w->firstTask;
  while (task || (task = getTaskFromQueue(w))) {
    try {
      task->taskFunc(task->args);
    } catch (std::exception& e) {
    }
    delete task;
    task = nullptr;
    if (!w->isCore) break;
  }
  if (!w->isCore) w->owner->removeWorker(w);
}

WorkerTask* Worker::getTaskFromQueue(Worker* w) {
  ThreadPool* owner = w->owner;
  std::unique_lock<std::mutex> lock(owner->mu);
  while (owner->tasks.empty() && w->isActive) {
    owner->cond.wait(lock);
  }
  if (!w->isActive) return nullptr;
  WorkerTask* task = owner->tasks.front();
  owner->tasks.pop();
  return task;
}
}  // namespace letMeSee
