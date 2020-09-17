#pragma once

#include <atomic>
#include <limits>
#include <stddef.h>
#include <utility>

#include "../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../uitsl/parallel/RelaxedAtomic.hpp"
#include "../../../../uitsl/utility/print_utils.hpp"
#include "../../../../uitsl/debug/WarnOnce.hpp"

namespace uit {
namespace a {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class AtomicSconceDuct {

  using T = typename ImplSpec::T;

  emp::array<T, 2> sconces{};
  std::atomic<size_t> position{};
  uitsl::RelaxedAtomic<size_t> updates_since_last_get;

public:

  AtomicSconceDuct() {
    static const uitsl::WarnOnce warning{
      "AtomicSconceDuct is experimental and may not be reliable"
    };
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    sconces[!position] = val;
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
    sconces[!position] = std::forward<P>(val);
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
    position ^= 1; // toggle between 0 and 1
    return updates_since_last_get.exchange(0);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return sconces[position]; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { return sconces[position]; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetType() { return "AtomicSconceDuct"; }

  static constexpr bool CanStep() { return false; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << std::endl;
    return ss.str();
  }

};

} // namespace a
} // namespace uit
