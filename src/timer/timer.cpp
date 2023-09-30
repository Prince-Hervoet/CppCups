#include "timer.hpp"

#include <chrono>

namespace letMeSee {
LongType Timer::GetNowTimestamp() {
  auto time = std::chrono::system_clock::now();
  auto timeMs = std::chrono::time_point_cast<std::chrono::milliseconds>(time);
  return timeMs.time_since_epoch().count();
}

bool Timer::Run() {
  if (repeatCount == 0) return false;
  if (repeatCount > 0) --repeatCount;
  taskFunc(taskArgs);
  if (repeatCount != 0) {
    this->willRunAt = GetNowTimestamp() + period;
  } else {
    isActive = false;
  }
  return true;
}

void Timer::SetTimer(std::function<void(void*)> taskFunc, void* args,
                     int period, int repeatCount) {
  this->taskFunc = std::move(taskFunc);
  this->period = period;
  this->repeatCount = repeatCount;
  this->taskArgs = args;
}
}  // namespace letMeSee