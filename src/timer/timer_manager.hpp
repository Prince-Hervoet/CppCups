#ifndef _TIMER_MANAGER_H_
#define _TIMER_MANAGER_H_

#include <iostream>
#include <map>
#include <thread>

#include "timer.hpp"

class TimerManager {
 private:
  std::multimap<size_t, Timer> timers;

 public:
  TimerManager() {}
  ~TimerManager() {}

  void updateTask() {
    size_t nowMs = Timer::GetCurrentTimestamps();
    while (!timers.empty()) {
      auto it = timers.begin();
      if (it->first > nowMs) break;
      Timer temp = it->second;
      timers.erase(it);
      temp.Execute();
      size_t currentCount = temp.GetCount();
      if (currentCount > 0) --currentCount;
      if (currentCount != 0) {
        nowMs = Timer::GetCurrentTimestamps();
        size_t nWillRunAt = nowMs + temp.GetPeriod();
        temp.SetCount(currentCount);
        timers.insert(std::pair<size_t, Timer>(nWillRunAt, temp));
      }
    }
  }

  void Register(std::function<void(void*)>& taskFunc, size_t period,
                size_t count) {
    if (period < 0 || count == 0) return;
    Timer t;
    t.SetPeriod(period);
    t.SetCount(count);
    t.SetTaskFunc(taskFunc);
    size_t willRunAt = Timer::GetCurrentTimestamps() + period;
    timers.insert(std::pair<size_t, Timer>(willRunAt, t));
  }

  size_t GetSize() { return timers.size(); }
};

#endif  // !