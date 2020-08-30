#pragma once

#include <stddef.h>

#include "../../../third-party/Empirical/source/base/assert.h"
#include "../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../conduit/config.hpp"
#include "../../parallel/OccupancyCaps.hpp"
#include "../../parallel/OccupancyGuard.hpp"
#include "../../utility/print_utils.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class HeadTailDuct {

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  // aligned implicit value initializes T
  using buffer_t = emp::array<T, N>;

  size_t head{};
  size_t tail{};
  buffer_t buffer;

  #ifndef NDEBUG
    mutable uit::OccupancyCaps caps;
  #endif

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t CountUnconsumedGets() const {
    //TODO FIXME handle wraparound case?
    emp_assert(tail <= head);
    return head - tail;
  }

public:

  /**
   * TODO.
   *
   * @param val TODO.
   */
  void Put(const T& val) {
    #ifndef NDEBUG
      const uit::OccupancyGuard guard{caps.Get("Put", 1)};
    #endif
    ++head;
    buffer[head % N] = val;
    emp_assert( CountUnconsumedGets() <= N );
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
  size_t TryConsumeGets(const size_t n) {
    #ifndef NDEBUG
      const uit::OccupancyGuard guard{caps.Get("ConsumeGets", 1)};
    #endif
    const size_t num_consumed = std::min( CountUnconsumedGets(), n );
    tail += num_consumed;
    return num_consumed;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return buffer[tail % N]; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetType() { return "HeadTailDuct"; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("size_t head", head);
    ss << format_member("size_t tail", tail);
    return ss.str();
  }


};

} // namespace uit
