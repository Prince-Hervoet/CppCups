#include "threadpool.hpp"

namespace letMeSee {
int ThreadPool::Commit(TaskType taskFunc, void* args) {
  taskBlock* task = new taskBlock();
  task->taskFunc = taskFunc;
  task->args = args;
  int ans = addTask(task);
  if (ans != 1) delete task;
  return ans;
}

int ThreadPool::addTask(taskBlock* task) {
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

bool ThreadPool::createThread(bool isCore, taskBlock* task) {
  std::unique_lock<std::mutex> lock(mu);
  if (!isActive) return false;
  if (isCore && coreSize == coreCapacity) return false;
}
}  // namespace letMeSee