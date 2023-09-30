#pragma once

#include <functional>
#include <map>
#include <thread>
#define LongType long long

namespace letMeSee {

class Timer;
class TimerManager {
 public:
  TimerManager() : isActive(false) {}
  ~TimerManager() {
    this->isActive = false;
    if (backendThread.joinable()) backendThread.join();
  }

  void AddTimer(std::function<void(void*)> taskFunc, void* args,
                LongType period, int repeatCount);
  void Start();
  void Stop();
  bool GetIsActive() { return isActive; }

 private:
  // 存放timer的容器
  std::multimap<LongType, Timer*> timers;
  volatile bool isActive;
  std::thread backendThread;
  void schedule();
  static void tmrRunThreadFunc(TimerManager* tmr);
};
}  // namespace letMeSee