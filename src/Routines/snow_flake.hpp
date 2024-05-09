#ifndef _SNOW_FLAKE_H_
#define _SNOW_FLAKE_H_

#include <chrono>
#include <mutex>

class Snowflake {
 private:
  std::mutex mtx;
  uint64_t lastTimestamp;
  const uint64_t epoch = 1609459200000;  // 2021-01-01 00:00:00 UTC
  const uint64_t workerIdBits = 5;
  const uint64_t datacenterIdBits = 5;
  const uint64_t sequenceBits = 12;
  const uint64_t maxWorkerId = -1 ^ (-1 << workerIdBits);
  const uint64_t maxDatacenterId = -1 ^ (-1 << datacenterIdBits);
  const uint64_t workerIdShift = sequenceBits;
  const uint64_t datacenterIdShift = sequenceBits + workerIdBits;
  const uint64_t timestampLeftShift =
      sequenceBits + workerIdBits + datacenterIdBits;
  const uint64_t sequenceMask = -1 ^ (-1 << sequenceBits);
  uint64_t workerId;
  uint64_t datacenterId;
  uint64_t sequence;

 public:
  Snowflake(uint64_t workerId, uint64_t datacenterId)
      : workerId(workerId),
        datacenterId(datacenterId),
        sequence(0),
        lastTimestamp(0) {
    if (workerId > maxWorkerId || workerId < 0) {
      throw std::invalid_argument("Worker ID must be between 0 and 31");
    }
    if (datacenterId > maxDatacenterId || datacenterId < 0) {
      throw std::invalid_argument("Datacenter ID must be between 0 and 31");
    }
  }

  uint64_t GetNextId() {
    std::lock_guard<std::mutex> lock(mtx);
    uint64_t timestamp = timeGen();
    if (timestamp < lastTimestamp) {
      throw std::runtime_error(
          "Clock moved backwards. Refusing to generate id for " +
          std::to_string(lastTimestamp - timestamp) + " milliseconds");
    }
    if (lastTimestamp == timestamp) {
      sequence = (sequence + 1) & sequenceMask;
      if (sequence == 0) {
        timestamp = tilNextMillis(lastTimestamp);
      }
    } else {
      sequence = 0;
    }
    lastTimestamp = timestamp;
    return ((timestamp - epoch) << timestampLeftShift) |
           (datacenterId << datacenterIdShift) | (workerId << workerIdShift) |
           sequence;
  }

 private:
  uint64_t tilNextMillis(uint64_t lastTimestamp) {
    uint64_t timestamp = timeGen();
    while (timestamp <= lastTimestamp) {
      timestamp = timeGen();
    }
    return timestamp;
  }

  uint64_t timeGen() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
  }
};

#endif
