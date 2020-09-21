#pragma once

#include <limits>
#include <stddef.h>
#include <utility>
#include <string>

#include "../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../uitsl/meta/f::static_test.hpp"
#include "../../../../uitsl/parallel/RelaxedAtomic.hpp"
#include "../../../../uitsl/utility/print_utils.hpp"

namespace uit {
namespace f {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class AtomicAccumulatingDuct {

  using T = typename ImplSpec::T;
  static_assert( uitsl::f::static_test<T>(), uitsl_f_message );

  uitsl::RelaxedAtomic<T> accumulator{};
  T cache{};

  size_t updates_since_last_get{};

public:

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    ++updates_since_last_get;
    accumulator += val;
    return true;
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() const { return true; }

  /**
   * TODO.
   *
   * @param requested TODO.
   */
  size_t TryConsumeGets(const size_t requested) {
    emp_assert( requested == std::numeric_limits<size_t>::max() );
    cache = accumulator.exchange( T{} );
    return std::exchange( updates_since_last_get, 0 );
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
  T& Get() { return cache; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetType() { return "AtomicAccumulatingDuct"; }

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

} // namespace f
} // namespace uit
