#pragma once

#include <stddef.h>

#include "base/assert.h"
#include "tools/string_utils.h"

#include "../../utility/CircularIndex.h"
#include "../../parallel/OccupancyCap.h"
#include "../../parallel/OccupancyGuard.h"

#include "../config.h"
#include "../../utility/print_utils.h"

namespace uit {

template<typename ImplSpec>
class Duct;

template<typename ImplSpec>
class PendingDuct {

  friend Duct<ImplSpec>;

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using pending_t = size_t;
  using buffer_t = emp::array<T, N>;

  pending_t pending{0};
  buffer_t buffer;

#ifndef NDEBUG
  mutable OccupancyCap cap{1};
#endif

public:

  void Initialize(const size_t write_position) { ; }

  //todo rename
  void Push() {

    #ifndef NDEBUG
      const OccupancyGuard guard{cap};
    #endif

    emp_assert(
      pending < N,
      [](){ error_message_mutex.lock(); return "locked"; }(),
      emp::to_string("pending: ", pending)
    );

    ++pending;
  }

  //todo rename
  void Pop(const size_t count) {

    #ifndef NDEBUG
      const OccupancyGuard guard{cap};
    #endif

    emp_assert(
      pending >= count,
      [](){ error_message_mutex.lock(); return "locked"; }(),
      emp::to_string("pending: ", pending),
      emp::to_string("count: ", count)
    );

    pending -= count;

  }

  size_t GetAvailableCapacity() { return N - GetPending(); }

  size_t GetPending() {

    #ifndef NDEBUG
      const OccupancyGuard guard{cap};
    #endif

    return pending;
  }

  T GetElement(const size_t n) const {

    #ifndef NDEBUG
      const OccupancyGuard guard{cap};
    #endif

    return buffer[n];
  }

  const void * GetPosition(const size_t n) const { return &buffer[n]; }

  void SetElement(const size_t n, const T & val) {

    #ifndef NDEBUG
      const OccupancyGuard guard{cap};
    #endif

    buffer[n] = val;
  }

  static std::string GetType() { return "PendingDuct"; }

  std::string ToString() const {

    #ifndef NDEBUG
      const OccupancyGuard guard{cap};
    #endif

    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("pending_t pending", pending);
    return ss.str();
  }


};

}
