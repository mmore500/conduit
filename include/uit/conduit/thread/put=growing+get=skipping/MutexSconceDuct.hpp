#pragma once

#include <mutex>
#include <limits>
#include <stddef.h>
#include <utility>

#include "../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../utility/print_utils.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class MutexSconceDuct {

  using T = typename ImplSpec::T;

  T sconce;
  T cache{};
  size_t updates_since_last_get{};

  mutable std::mutex mutex;

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t CountUnconsumedGets() const { return updates_since_last_get; }

public:

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    const std::lock_guard guard{ mutex };
    sconce = val;
    ++updates_since_last_get;
    return true;
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  template<typename P>
  bool TryPut(P&& val) {
    const std::lock_guard guard{ mutex };
    sconce = std::forward<P>(val);
    ++updates_since_last_get;
    return true;
  }

  /**
   * TODO.
   *
   * @param n TODO.
   */
  size_t TryConsumeGets(const size_t requested) {
    emp_assert( requested == std::numeric_limits<size_t>::max() );
    const std::lock_guard guard{ mutex };
    cache = sconce;
    return std::exchange(updates_since_last_get, 0);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return cache; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetType() { return "MutexSconceDuct"; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    return ss.str();
  }

};

} // namespace uit
