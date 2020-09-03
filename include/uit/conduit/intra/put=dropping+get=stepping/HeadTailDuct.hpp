#pragma once

#include <stddef.h>

#include "../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../conduit/config.hpp"
#include "../../../debug/occupancy_audit.hpp"
#include "../../../utility/print_utils.hpp"

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

  uit_occupancy_auditor;

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

  /**
   * TODO.
   *
   * @param val TODO.
   */
  void DoPut(const T& val) {
    uit_occupancy_audit(1);
    ++head;
    buffer[head % N] = val;
    emp_assert( CountUnconsumedGets() <= N );
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  template<typename P>
  void DoPut(P&& val) {
    uit_occupancy_audit(1);
    ++head;
    buffer[head % N] = std::forward<P>(val);
    emp_assert( CountUnconsumedGets() <= N );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  bool IsReadyForPut() const { return CountUnconsumedGets() < N; }

public:

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    if (IsReadyForPut()) { DoPut(val); return true; }
    else return false;
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  template<typename P>
  bool TryPut(P&& val) {
    if (IsReadyForPut()) { DoPut(std::forward<P>(val)); return true; }
    else return false;
  }

  /**
   * TODO.
   *
   * @param n TODO.
   */
  size_t TryConsumeGets(const size_t n) {
    uit_occupancy_audit(1);
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
  T& Get() { return buffer[tail % N]; }

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
