#include <atomic>
#include <iostream>
#include <vector>

#include "macros.h"

namespace Common {
template <typename T>
// Lock-free queue for single producer single consumer
// queue operations. Must call update methods after read or
// write operations.
// TODO: Make this more robust. Consumer and producer
// shouldn't be required to call the updates on tracking
// variables.
class LfQueue final {
 public:
  LfQueue(size_t queue_size) : store_(queue_size, T()), max_size_{queue_size} {}

  LfQueue() = delete;
  LfQueue(const LfQueue&) = delete;
  LfQueue(const LfQueue&&) = delete;
  LfQueue& operator=(const LfQueue&) = delete;
  LfQueue& operator=(const LfQueue&&) = delete;

  // Return a pointer to the next write. Returns nullptr
  // if the queue is full.
  T* getNextWriteTo() noexcept {
    // Prevent adding to a full queue non consumed elements.
    if (size() == max_size_) [[unlikely]]
      return nullptr;
    return &store_[next_write_i_];
  }

  // Update 'next_write_i_' after a write.
  void updateNextWrite() noexcept {
    next_write_i_ = (next_write_i_ + 1) % store_.size();
    elem_count_++;
  }

  const T* getNextRead() const noexcept {
    // Empty queue returns nullptr
    return (size() ? &store_[next_read_i_] : nullptr);
  }

  void updateNextRead() noexcept {
    next_read_i_ = (next_read_i_ + 1) % store_.size();
    ASSERT(elem_count_.load() != 0, "Invalid read in LfQueue.");
    elem_count_--;
  }

  size_t size() const noexcept { return elem_count_.load(); }

 private:
  std::vector<T> store_;
  std::atomic<size_t> next_write_i_{0};
  std::atomic<size_t> next_read_i_{0};
  std::atomic<size_t> elem_count_{0};
  const size_t max_size_;
};
}  // namespace Common
