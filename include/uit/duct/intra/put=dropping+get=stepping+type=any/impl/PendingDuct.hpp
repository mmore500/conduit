#pragma once

#include <stddef.h>

#include "../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../../uitsl/debug/occupancy_audit.hpp"
#include "../../../../../uitsl/nonce/CircularIndex.hpp"
#include "../../../../../uitsl/utility/print_utils.hpp"

namespace uit {
namespace internal {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<
  typename PendingType,
  typename BufferElementType,
  typename ImplSpec
>
class PendingDuct {

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using buffer_t = emp::array<BufferElementType, N>;

  PendingType pending_gets{};
  uitsl::CircularIndex<N> put_position{1};
  uitsl::CircularIndex<N> get_position{};
  buffer_t buffer{};

  uit_occupancy_auditor;

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t CountUnconsumedGets() const { return pending_gets; }

  void DoPut(const T& val) {
    uit_occupancy_audit(1);
    buffer[put_position] = val;
    ++pending_gets;
    ++put_position;
    emp_assert( pending_gets <= N );
  }

  template<typename P>
  void DoPut(P&& val) {
    uit_occupancy_audit(1);
    buffer[put_position] = std::forward<P>(val);
    ++pending_gets;
    ++put_position;
    emp_assert( pending_gets <= N );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  bool IsReadyForPut() const { return pending_gets < N; }

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
  bool Put(P&& val) {
    if (IsReadyForPut()) { DoPut( std::forward<P>(val) ); return true; }
    else return false;
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() const { return true; }

  /**
   * TODO.
   *
   * @param requested.
   * @return num consumed.
   */
  size_t TryConsumeGets(const size_t requested) {
    uit_occupancy_audit(1);
    const size_t num_consumed = std::min( requested, CountUnconsumedGets() );
    get_position += num_consumed;
    pending_gets -= num_consumed;
    return num_consumed;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return buffer[get_position]; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { return buffer[get_position]; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "PendingDuct"; }

  static constexpr bool CanStep() { return true; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << std::endl;
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << uitsl::format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << uitsl::format_member("pending_t pending_gets", pending_gets);
    ss << uitsl::format_member("pending_t get_position", get_position);
    return ss.str();
  }


};

} // namespace internal
} // namespace uit
