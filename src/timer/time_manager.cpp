#include <iostream>

#include "timer.hpp"
#include "timer_manager.hpp"

namespace letMeSee {

void TimerManager::schedule() {
  LongType nowTimestamp = Timer::GetNowTimestamp();
  while (!timers.empty()) {
    auto min = timers.begin();
    if (min->first > nowTimestamp) break;
    Timer* timer = min->second;
    timers.erase(min);
    timer->Run();
    if (timer->GetRepeatCount() == 0) {
      delete timer;
    } else {
      LongType key = timer->GetWillRunAt();
      timers.insert(std::pair<LongType, Timer*>(key, timer));
    }
  }
  if (timers.empty()) isActive = false;
}

void TimerManager::tmrRunThreadFunc(TimerManager* tmr) {
  while (tmr->isActive) {
    try {
      tmr->schedule();
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}

void TimerManager::AddTimer(std::function<void(void*)> taskFunc, void* args,
                            LongType period, int repeatCount) {
  Timer* timer = new Timer();
  timer->SetTimer(taskFunc, args, period, repeatCount);
  LongType key = Timer::GetNowTimestamp();
  timers.insert(std::pair<LongType, Timer*>(key, timer));
}

void TimerManager::Start() {
  if (isActive) {
    return;
  }
  isActive = true;
  backendThread = std::thread(tmrRunThreadFunc, this);
  backendThread.detach();
}

void TimerManager::Stop() { isActive = false; }
}  // namespace letMeSee