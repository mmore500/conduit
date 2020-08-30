#pragma once

#include <mutex>
#include <stddef.h>

#include "../../../third-party/Empirical/source/base/assert.h"
#include "../../../third-party/Empirical/source/base/errors.h"
#include "../../../third-party/Empirical/source/tools/string_utils.h"
#include "../../../third-party/SPSCQueue/include/rigtorp/SPSCQueue.h"

#include "../../parallel/RelaxedAtomic.hpp"
#include "../../utility/print_utils.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class RigtorpDuct {

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  rigtorp::SPSCQueue<T> queue{N};

  using pending_t = uit::RelaxedAtomic<size_t>;

  #ifndef NDEBUG
    mutable uit::OccupancyCaps caps;
  #endif

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t CountUnconsumedGets() const {
    const size_t available = queue.size();
    emp_assert( available );
    return available - 1;
  }

public:

  RigtorpDuct() { queue.push( T{} ); }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  void Put(const T& val) {
    #ifndef NDEBUG
      const uit::OccupancyGuard guard{caps.Get("Put", 1)};
    #endif
    queue.push( val );
    emp_assert(CountUnconsumedGets() <= N);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  // TODO why N - 1?
  bool IsReadyForPut() const { return CountUnconsumedGets() < N - 1; }

  /**
   * TODO.
   *
   * @param n TODO.
   */
  size_t TryConsumeGets(const size_t requested) {
    #ifndef NDEBUG
      const uit::OccupancyGuard guard{caps.Get("TryConsumeGets", 1)};
    #endif

    const size_t num_consumed = std::min( requested, CountUnconsumedGets() );
    for (size_t i = 0; i < num_consumed; ++i) queue.pop();
    return num_consumed;

  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() { return *queue.front(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetType() { return "RigtorpDuct"; }

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
