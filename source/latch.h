#pragma once

// polyfill until C++20 barrier becomes available
// TODO C++20 cpp20 switch to std::barrier

// adapted from
// https://github.com/llvm/llvm-project/blob/bcf14f375e29b94e7abb381920df795eeefb2309/libcxx/include/latch

#include <atomic>
#include <cstddef>
#include <condition_variable>

class latch
{
  std::atomic<ptrdiff_t> arrived;

  mutable std::condition_variable cv;
  mutable std::mutex mutex;

public:
  static constexpr ptrdiff_t max() noexcept {
    return std::numeric_limits<ptrdiff_t>::max();
  }

  inline explicit latch(ptrdiff_t expected)
  : arrived(expected)
  { ; }

  ~latch() = default;
  latch(const latch&) = delete;
  latch& operator=(const latch&) = delete;

  inline void count_down(ptrdiff_t update = 1) {
    auto const old = arrived.fetch_sub(update, std::memory_order_release);
    if (old == update) cv.notify_all();
  }

  inline bool try_wait() const noexcept {
    return 0 == arrived.load(std::memory_order_acquire);
  }

  inline void wait() const
  {
    auto const test_function = [this]() -> bool {
      return try_wait();
    };

    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, test_function);

  }

  inline void arrive_and_wait(ptrdiff_t update = 1)
  {
    count_down(update);
    wait();
  }
};
