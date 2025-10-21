#pragma once

#include <chrono>
#include <ctime>
#include <format>
#include <string>

namespace Common {

typedef int64_t nanos;
constexpr nanos NANOS_TO_MICROS = 1000;
constexpr nanos MICROS_TO_MILLIS = 1000;
constexpr nanos MILLIS_TO_SECS = 1000;
constexpr nanos NANOS_TO_MILLIS = NANOS_TO_MICROS * MICROS_TO_MILLIS;
constexpr nanos NANOS_TO_SECS = NANOS_TO_MILLIS * MILLIS_TO_SECS;

// Simple clock interface to allow using fake clock in testing
class Clock {
 public:
  virtual ~Clock() = default;
  virtual std::chrono::system_clock::time_point now() const = 0;
};

// Uses system clock
class SystemClock : public Clock {
 public:
  std::chrono::system_clock::time_point now() const override {
    return std::chrono::system_clock::now();
  }
};

inline nanos getCurrentNanos(const Clock& clock = SystemClock()) noexcept {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             clock.now().time_since_epoch())
      .count();
}

inline std::string& getCurrentTimeStr(
    std::string* time_str, const Clock& clock = SystemClock()) noexcept {
  const auto now = clock.now();
  time_str->assign(std::format("{:%a %b %e %H:%M:%S %Y}", now));
  return *time_str;
}

}  // namespace Common
