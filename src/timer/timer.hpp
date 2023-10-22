#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>
#include <functional>

/**
 * 定时器结构
 */
class Timer {
 private:
  size_t willRunAt;  // 目标执行时间点
  size_t activeAt;
  size_t runPeriod;  // 执行周期时间戳
  size_t runCount;   // 定时器触发次数
  bool active;
  std::function<void(void*)> taskFunc;

 public:
  Timer() {}

  ~Timer() {}

  static size_t GetCurrentTimestamps() {
    auto now = std::chrono::system_clock::now();
    auto nowMs = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    return nowMs.time_since_epoch().count();
  }

  void SetPeriod(size_t period) { runPeriod = period; }

  void SetCount(size_t count) { runCount = count; }

  void SetActive(bool active) {
    this->active = active;
    activeAt = Timer::GetCurrentTimestamps();
  }

  void SetTaskFunc(std::function<void(void*)>& taskFunc) {
    this->taskFunc = taskFunc;
  }

  void SetWillRunAt(size_t willRunAt) { this->willRunAt = willRunAt; }

  bool IsActive() { return active; }

  size_t GetCount() { return runCount; }

  size_t GetPeriod() { return runPeriod; }

  void Execute() { taskFunc(nullptr); }
};

#endif  // !