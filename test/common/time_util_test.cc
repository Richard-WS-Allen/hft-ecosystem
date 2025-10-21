#include "time_util.h"

#include <gtest/gtest.h>

namespace Common {

// Fake clock to use for testing
class FakeClock : public Clock {
 private:
  std::chrono::system_clock::time_point current_time_point;

 public:
  FakeClock(std::chrono::system_clock::time_point start_time)
      : current_time_point(start_time) {}

  std::chrono::system_clock::time_point now() const override {
    return current_time_point;
  }

  void advance(std::chrono::nanoseconds duration) {
    current_time_point += duration;
  }
};

namespace {

TEST(TimeUtilsTest, TestNanos) {
  auto start_time = std::chrono::system_clock::time_point{};  // Epoch
  FakeClock clock(start_time);

  nanos initial_nanos = getCurrentNanos(clock);
  EXPECT_EQ(initial_nanos, 0);  // Starting at epoch

  clock.advance(std::chrono::milliseconds(100));

  nanos new_nanos = getCurrentNanos(clock);
  EXPECT_EQ(new_nanos - initial_nanos, 100'000'000);  // 100ms in nanoseconds
}

TEST(TimeUtilsTest, TestTimeStringFormatting) {
  std::string time_buffer;
  auto known_time_point =
      std::chrono::system_clock::from_time_t(0);  // unix epoch
  FakeClock clock(known_time_point);

  getCurrentTimeStr(&time_buffer, clock);
  EXPECT_EQ(time_buffer, "Thu Jan  1 00:00:00.000000000 1970");

  clock.advance(std::chrono::seconds(60 * 60 * 24));  // one day
  getCurrentTimeStr(&time_buffer, clock);
  EXPECT_EQ(time_buffer, "Fri Jan  2 00:00:00.000000000 1970");
}

}  // namespace
}  // namespace Common
