#include "thread_pool.hpp"

#include <cstddef>
#include <exception>
#include <iostream>
#include <mutex>

namespace let_me_see {
int ThreadPool::Commit(TaskType func, ArgsType args) {
  if (func == nullptr || status == STOP) return -1;
  int current = thread_size.load();
  if (current < max_core_thread_size) {
    if (createThread(func, args, true)) return 1;
    if (status == STOP) return -1;
  }
  if (task_queue.size() < max_task_size) {
    if (addTaskToList(func, args)) return 1;
    if (status == STOP) return -1;
  }
  if (current < max_thread_size) {
    if (createThread(func, args, false)) return 1;
    if (status == STOP) return -1;
  }
  return -2;
}

void ThreadPool::Close() {
  {
    std::unique_lock<std::mutex> lock(mu);
    status = STOP;
    cond.notify_all();
  }
  WorkerPtr worker = nullptr;
  for (auto it = worker_set.begin(); it != worker_set.end(); ++it) {
    worker = *it;
    if (worker->joinable()) worker->join();
    delete worker;
  }
  worker_set.clear();
}

bool ThreadPool::createThread(TaskType func, ArgsType args, bool is_core) {
  for (;;) {
    int current = thread_size.load();
    if (current >= (is_core ? max_core_thread_size : max_thread_size) ||
        status == STOP)
      return false;
    if (thread_size.compare_exchange_weak(current, current + 1)) break;
  }
  {
    std::unique_lock<std::mutex> lock(mu);
    if (status == STOP) return false;
    WorkerPtr nWorker = new Worker();
    nWorker->is_core = is_core;
    TaskObjectPtr task = new TaskObject{.func = func, .args = args};
    nWorker->task = task;
    worker_set.insert(nWorker);
    nWorker->start(this);
  }
  return true;
}

bool ThreadPool::addTaskToList(TaskType func, ArgsType args) {
  std::unique_lock<std::mutex> lock(mu);
  if (status == STOP) return false;
  if (task_queue.size() >= max_task_size) return false;
  TaskObjectPtr task = new TaskObject{.func = func, .args = args};
  task_queue.push(task);
  cond.notify_one();
  return true;
}

TaskObjectPtr ThreadPool::getTaskFromQueue() {
  std::unique_lock<std::mutex> lock(mu);
  if (status == STOP) return nullptr;
  while (task_queue.empty()) {
    cond.wait(lock);
    if (status == STOP) return nullptr;
  }
  TaskObjectPtr to = task_queue.front();
  task_queue.pop();
  return to;
}

void ThreadPool::closeWorker(WorkerPtr worker) {
  if (worker == nullptr) return;
  std::unique_lock<std::mutex> lock(mu);
  if (status == STOP) return;
  worker_set.erase(worker);
  delete worker;
}

void Worker::innerThreadFunc(WorkerPtr self, ThreadPool* tp) {
  TaskObjectPtr task = self->task;
  int count = 0;
  while (tp->status != STOP &&
         (task != nullptr || (task = tp->getTaskFromQueue()))) {
    try {
      task->func(task->args);
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
    delete task;
    task = nullptr;
    if (self->is_core) continue;
    count++;
    if (count == NO_CORE_TRY_COUNT) break;
  }
  if (tp->status != STOP) tp->closeWorker(self);
}

void Worker::start(ThreadPool* tp) {
  if (is_runned) return;
  is_runned = true;
  thread = std::thread(Worker::innerThreadFunc, this, tp);
}

}  // namespace let_me_see