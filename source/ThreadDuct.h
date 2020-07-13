#pragma once

#include "base/assert.h"
#include "tools/string_utils.h"

#include "print_utils.h"

template<typename T, size_t N>
class Duct;

template<typename T, size_t N>
class ThreadDuct {

  friend Duct<T, N>;

  using pending_t = std::atomic<size_t>;
  using buffer_t = std::array<T, N>;

  pending_t pending{0};
  buffer_t buffer;

public:

  //todo rename
  void Push() {

    emp_assert(
      pending < N,
      [](){ error_message_mutex.lock(); return "locked"; }(),
      emp::to_string("pending: ", pending)
    );

    pending.fetch_add(1, std::memory_order_relaxed);
  }

  //todo rename
  void Pop(const size_t count) {

    emp_assert(
      pending >= count,
      [](){ error_message_mutex.lock(); return "locked"; }(),
      emp::to_string("pending: ", pending),
      emp::to_string("count: ", count)
    );

    pending.fetch_sub(count, std::memory_order_relaxed);
  }

  size_t GetPending() const { return pending; }

  size_t GetAvailableCapacity() const { return N - pending; }

  T GetElement(const size_t n) const { return buffer[n]; }

  void SetElement(const size_t n, const T & val) { buffer[n] = val; }

  std::string ToString() const {
    std::stringstream ss;
    ss << "ThreadDuct" << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("pending_t pending", (size_t) pending);
    return ss.str();
  }


};
