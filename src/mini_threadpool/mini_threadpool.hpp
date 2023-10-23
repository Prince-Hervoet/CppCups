#ifndef _MINI_THREADPOOL_H_
#define _MINI_THREADPOOL_H_

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace let_me_see {
class Task {
 public:
  Task(std::function<void(void*)> task, void* args) : task(task), args(args) {}
  std::function<void(void*)> task;
  void* args;
};

using Task = struct Task;

class ThreadPool {
 public:
  ThreadPool(int thread_size) {}
  ~ThreadPool() {
    {
      std::unique_lock<std::mutex> lock(mu);
      stop = true;
    }
    cond.notify_all();
    for (int i = 0; i < threads.size(); ++i) {
      if (threads[i].joinable()) {
        threads[i].join();
      }
    }
  }

  void AddTask(std::function<void(void*)>& task, void* args) {
    {
      std::unique_lock<std::mutex> lock(mu);
      auto t = std::bind(task, args);
      tasks.emplace(t);
    }
    cond.notify_one();
  }

 private:
  void addThread() {
    threads.emplace_back([this]() {
      for (;;) {
        {
          std::unique_lock<std::mutex> lock(mu);
          while (tasks.empty() && !stop) {
            cond.wait(lock);
          }
          if (stop) break;
          std::function<void(void*)> task = std::move(tasks.front());
          tasks.pop();
          task(nullptr);
        }
      }
    });
  }

  bool stop;
  std::condition_variable cond;
  std::mutex mu;
  std::queue<std::function<void(void*)>> tasks;
  std::vector<std::thread> threads;
};
}  // namespace let_me_see

#endif  // !