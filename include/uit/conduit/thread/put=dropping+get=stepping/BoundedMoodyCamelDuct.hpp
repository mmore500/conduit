#pragma once

#include <mutex>
#include <stddef.h>

#include "../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../third-party/Empirical/source/base/errors.h"
#include "../../../../../third-party/Empirical/source/tools/string_utils.h"
#include "../../../../../third-party/readerwriterqueue/atomicops.h"
#include "../../../../../third-party/readerwriterqueue/readerwriterqueue.h"

#include "../../../utility/print_utils.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class BoundedMoodyCamelDuct {

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  moodycamel::ReaderWriterQueue<T> queue{N};

  #ifndef NDEBUG
    mutable uit::OccupancyCaps caps;
  #endif

  size_t CountUnconsumedGets() const {
    const size_t available = queue.size_approx();
    emp_assert( available );
    return available - 1;
  }

public:

  BoundedMoodyCamelDuct() { queue.enqueue( T{} ); }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  void Put(const T& val) {
    #ifndef NDEBUG
      const uit::OccupancyGuard guard{caps.Get("Put", 1)};
    #endif
     queue.enqueue( val );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  bool IsReadyForPut() const { return CountUnconsumedGets() < N; }

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
  const T& Get() { return *queue.peek(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetType() { return "BoundedMoodyCamelDuct"; }

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
