#include "threadpool.hpp"

namespace letMeSee {
int ThreadPool::Commit(TaskType taskFunc, void* args) {
  WorkerTask* task = new WorkerTask();
  task->taskFunc = taskFunc;
  task->args = args;
  int ans = addTask(task);
  if (ans != 1) delete task;
  return ans;
}

int ThreadPool::addTask(WorkerTask* task) {
  if (!isActive) return 0;
  if (coreSize < coreCapacity) {
    if (createThread(true, task)) return 1;
    if (!isActive) return 0;
  }
  if (addTaskToQueue(task, true)) return 1;
  if (!isActive) return 0;
  if (createThread(false, task)) return 1;
  if (!isActive) return 0;
  addTaskToQueue(task, false);
  return 1;
}

bool ThreadPool::addTaskToQueue(WorkerTask* task, bool isTry) {
  if (!isActive) return false;
  std::unique_lock<std::mutex> lock(mu);
  while (tasks.size() == taskCapacity && isActive) {
    if (isTry) return false;
    addCond.wait(lock);
  }
  if (!isActive) return false;
  tasks.emplace(task);
  return true;
}

bool ThreadPool::createThread(bool isCore, WorkerTask* task) {
  std::unique_lock<std::mutex> lock(mu);
  if (!isActive) return false;
  if (isCore && coreSize == coreCapacity) return false;
  if (threadSize == threadCapacity) return false;
  Worker* w = new Worker(this, task);
  if (isCore) coreSize += 1;
  threadSize += 1;
  w->Start();
}

void ThreadPool::Stop() {
  if (!isActive) return;
  {
    std::unique_lock<std::mutex> lock(mu);
    if (!isActive) return;
    isActive = false;
    for (auto it = threads.begin(); it != threads.end(); ++it) (*it)->Stop();
    addCond.notify_all();
    cond.notify_all();
  }
  for (auto it = threads.begin(); it != threads.end(); ++it) {
    (*it)->Join();
    delete (*it);
  }
  threads.clear();
}

void ThreadPool::removeWorker(Worker* w) {
  if (w == nullptr) return;
  std::unique_lock<std::mutex> lock(mu);
  if (!isActive) return;
  w->Detach();
  threads.erase(w);
  delete w;
}
}  // namespace letMeSee