#pragma once

#include <atomic>
#include <stddef.h>

#include "../../../third-party/Empirical/source/base/assert.h"
#include "../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../conduit/config.hpp"
#include "../../utility/print_utils.hpp"

namespace uit {

template<typename ImplSpec>
class Duct;

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class AtomicPendingDuct {

  friend Duct<ImplSpec>;

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  struct alignas(CACHE_LINE_SIZE) padded {
    T t;
    std::string ToString() const { return emp::to_string(t); }
  };

  struct alignas(CACHE_LINE_SIZE) pending_t : public std::atomic<size_t> { };
  struct alignas(CACHE_LINE_SIZE) buffer_t : public emp::array<padded, N> { };

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

  size_t GetPending() { return pending; }

  size_t GetAvailableCapacity() { return N - GetPending(); }

  T GetElement(const size_t n) const { return buffer[n].t; }

  const void * GetPosition(const size_t n) const { return &buffer[n].t; }

  void SetElement(const size_t n, const T & val) { buffer[n].t = val; }

  static std::string GetType() { return "AtomicPendingDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("pending_t pending", (size_t) pending);
    return ss.str();
  }


};

} // namespace uit
