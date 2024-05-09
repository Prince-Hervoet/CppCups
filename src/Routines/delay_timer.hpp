#ifndef _DELAY_TIMER_H_
#define _DELAY_TIMER_H_

#include <any>
#include <cstddef>
#include <cstdint>
#include <map>

namespace let_me_see {

using ArgsType = std::any;
using TimerTaskType = void (*)(ArgsType args);
class DelayTimer;
class Timer {
  friend class DelayTimer;

 private:
  int repeat = 0;
  size_t delay;
  size_t create_at;
  size_t next_run_at;
  TimerTaskType func;
  ArgsType args;

 public:
  void Run() { func(args); }
};

class DelayTimer {
 public:
  using TimerType = Timer;
  using TimerPtr = Timer*;

 private:
  std::multimap<size_t, Timer> timer_map;

 public:
  void Schedule(TimerTaskType func, ArgsType args, uint64_t delay, int repeat);
  bool CheckTimer();
};
}  // namespace let_me_see

#endif