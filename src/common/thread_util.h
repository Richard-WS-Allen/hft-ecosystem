#pragma once

#include <sys/syscall.h>
#include <unistd.h>

#include <atomic>
#include <iostream>
#include <thread>

#include "macros.h"

namespace Common {
// Set affinity for current thread to the 'core_id'.
inline bool setThreadCore(const int core_id) noexcept {
  cpu_set_t cpuset;
  // Zero cpu set (bitmask) and add core_id to zeroed set
  // https://man7.org/linux/man-pages/man3/CPU_SET.3.html
  CPU_ZERO(&cpuset);
  CPU_SET(core_id, &cpuset);

  // Set the current thread affinity and return true if successful.
  return (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) ==
          0);
}

// Starts a thread for 'func' and it's given 'args' and assigns 'name'.
// Sets affinity on the thread when 'core_id' is non-negative.
template <typename T, typename... A>
inline std::thread createAndStartThread(const int core_id,
                                        const std::string& name, T&& func,
                                        A&&... args) noexcept {
  std::thread t = new std::thread([&]() {
    if (core_id >= 0 && !setThreadCore(core_id)) {
      std::cerr << "Failed to set core affinity for " << name << " to core "
                << core_id << std::endl;
      exit(EXIT_FAILURE);
    }

    std::forward<T>(func)((std::forward<A>(args))...);
  });
  return t;
}

}  // namespace Common
