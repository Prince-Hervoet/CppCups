#include "delay_timer.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <utility>

namespace let_me_see {

static size_t getCurrentTimestamp() {
  auto now = std::chrono::system_clock::now();
  auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                       now.time_since_epoch())
                       .count();
  return timestamp;
}

void DelayTimer::Schedule(TimerTaskType func, ArgsType args, uint64_t delay,
                          int repeat) {
  if (func == nullptr) return;
  TimerType timer;
  timer.func = func;
  timer.args = args;
  timer.delay = delay;
  timer.repeat = repeat;
  timer.create_at = getCurrentTimestamp();
  timer.next_run_at = timer.create_at + delay;
  timer_map.insert(std::pair<size_t, Timer>(timer.next_run_at, timer));
}

bool DelayTimer::CheckTimer() {
  size_t now_time = getCurrentTimestamp();
  bool is_run = false;
  while (!timer_map.empty()) {
    auto it = timer_map.begin();
    Timer& timer = it->second;
    if (now_time < timer.next_run_at) return is_run;
    timer.Run();
    is_run = true;
    if (timer.repeat == 0) {
      timer_map.erase(it);
      continue;
    }
    timer.next_run_at = timer.create_at + timer.delay;
    timer_map.insert(std::pair<size_t, Timer>(timer.next_run_at, timer));
    timer_map.erase(it);
    if (timer.repeat > 0) timer.repeat--;
  }
  return is_run;
}
}  // namespace let_me_see