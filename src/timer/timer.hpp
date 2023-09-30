#pragma once

#include <functional>
#define LongType long long

namespace letMeSee {
class Timer {
 public:
  Timer() {}
  ~Timer() {}

  void SetTimer(std::function<void(void*)> taskFunc, void* args, int period,
                int repeatCount);
  bool Run();
  LongType GetWillRunAt() { return willRunAt; }
  int GetRepeatCount() { return repeatCount; }
  void SetRepeatCount(int value) { repeatCount = value; }
  bool GetIsActive() { return isActive; }
  static LongType GetNowTimestamp();

 private:
  LongType willRunAt;
  LongType period;
  int repeatCount;
  bool isActive;
  std::function<void(void*)> taskFunc;
  void* taskArgs;
};
}  // namespace letMeSee