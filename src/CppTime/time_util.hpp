#ifndef _TIME_UTIL_H_
#define _TIME_UTIL_H_

#include <_timeval.h>

#include <chrono>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace let_me_see {
class TimeUtil {
  using TimePointType = std::chrono::high_resolution_clock::time_point;

 public:
  static uint64_t GetCurrentTimeNanos() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration)
        .count();
  }

  static uint64_t GetCurrentTimeMillis() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration)
        .count();
  }

  static uint64_t GetCurrentTimeSeconds() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
  }

  static std::string GetCurrentTimeString(
      const std::string& format = "%Y-%m-%d %H:%M:%S") {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* tm_now = std::localtime(&now_time);
    std::ostringstream oss;
    oss << std::put_time(tm_now, format.c_str());
    return oss.str();
  }

 public:
  class StopWatch {
   private:
    TimePointType start;
    TimePointType end;

   public:
    void Start() { start = std::chrono::high_resolution_clock::now(); }

    void Stop() { end = std::chrono::high_resolution_clock::now(); }

    void Reset() {
      start = std::chrono::high_resolution_clock::time_point::min();
      end = std::chrono::high_resolution_clock::time_point::min();
    }

    uint64_t GetElapsedNanos() const {
      return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
          .count();
    }

    uint64_t GetElapsedMillis() const {
      return std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();
    }

    uint64_t GetElapsedSeconds() const {
      return std::chrono::duration_cast<std::chrono::seconds>(end - start)
          .count();
    }
  };
};
}  // namespace let_me_see

#endif
