#pragma once

#include "base/assert.h"
#include "tools/string_utils.h"

#include "config_utils.h"
#include "print_utils.h"

template<typename T, size_t N>
class Duct;

template<typename T, size_t N=DEFAULT_BUFFER>
class ThreadDuct {

  friend Duct<T, N>;

  struct alignas(CACHE_LINE_SIZE) padded {
    T t;
    std::string ToString() const { return emp::to_string(t); }
  };

  struct alignas(CACHE_LINE_SIZE) pending_t : public std::atomic<size_t> { };
  struct alignas(CACHE_LINE_SIZE) buffer_t : public std::array<padded, N> { };

  pending_t pending{0};
  buffer_t buffer;

public:

  void Initialize(const size_t write_position) { ; }

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

  T GetElement(const size_t n) const { return buffer[n].t; }

  void SetElement(const size_t n, const T & val) { buffer[n].t = val; }

  std::string GetType() const { return "ThreadDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("pending_t pending", (size_t) pending);
    return ss.str();
  }


};
