#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <set>
#include <thread>

namespace let_me_see {
#define DEFAULT_MAX_THREAD_SIZE (536870911)
#define DEFAULT_CORE_THREAD_SIZE (32)
#define DEFAULT_MAX_TASK_SIZE (1024)
#define NO_CORE_TRY_COUNT 3

#define RUNNING 1
#define STOP 2

using ArgsType = void*;
using TaskType = void (*)(ArgsType);
class ThreadPool;
typedef struct TaskObject {
  TaskType func;
  ArgsType args;
} TaskObject;

using TaskObjectPtr = TaskObject*;

class Worker {
 public:
  friend class ThreadPool;
  using WorkerPtr = Worker*;

 private:
  TaskObjectPtr task;
  std::thread thread;
  bool is_core;
  bool is_runned = false;

 public:
  ~Worker() {
    if (task) delete task;
  }

 private:
  void start(ThreadPool* tp);
  bool joinable() { return thread.joinable(); }
  void join() { thread.join(); }
  static void innerThreadFunc(WorkerPtr self, ThreadPool* tp);
};

class ThreadPool {
  friend class Worker;
  using ThreadPoolPtr = ThreadPool*;
  using WorkerPtr = Worker*;

 private:
  char status = STOP;
  int max_thread_size = 0;
  int max_core_thread_size = 0;
  int max_task_size = 0;
  std::atomic<int> thread_size{0};
  std::set<WorkerPtr> worker_set;
  std::queue<TaskObjectPtr> task_queue;
  std::mutex mu;
  std::condition_variable cond;

 public:
  ThreadPool()
      : max_thread_size(DEFAULT_MAX_THREAD_SIZE),
        max_core_thread_size(DEFAULT_CORE_THREAD_SIZE),
        max_task_size(DEFAULT_MAX_TASK_SIZE) {}

  ThreadPool(int max_thread_size, int max_core_thread_size, int max_task_size)
      : max_thread_size(max_task_size),
        max_core_thread_size(max_core_thread_size),
        max_task_size(max_task_size) {}

 public:
  int Commit(TaskType func, ArgsType args);
  void Close();
  char Status() { return status; }
  int Size() { return thread_size.load(); }

 private:
  bool createThread(TaskType func, ArgsType args, bool is_core);
  bool addTaskToList(TaskType func, ArgsType args);
  void closeWorker(WorkerPtr worker);
  TaskObjectPtr getTaskFromQueue();
  static void innerThreadRun(ThreadPoolPtr tp);
};

using WorkerPtr = Worker*;
}  // namespace let_me_see

#endif