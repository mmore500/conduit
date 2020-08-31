#pragma once

#include <stddef.h>

#include "../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../parallel/OccupancyCaps.hpp"
#include "../../../../parallel/OccupancyGuard.hpp"
#include "../../../../utility/CircularIndex.hpp"
#include "../../../../utility/print_utils.hpp"

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
  uit::CircularIndex<N> put_position{1};
  uit::CircularIndex<N> get_position{};
  buffer_t buffer{};

  #ifndef NDEBUG
    mutable uit::OccupancyCaps caps;
  #endif

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t CountUnconsumedGets() const { return pending_gets; }


public:

  /**
   * TODO.
   *
   * @param val TODO.
   */
  void Put(const T& val) {
    #ifndef NDEBUG
      const OccupancyGuard guard{caps.Get("Put", 1)};
    #endif
    buffer[put_position] = val;
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

  /**
   * TODO.
   *
   * @param requested.
   * @return num consumed.
   */
  size_t TryConsumeGets(const size_t requested) {
    #ifndef NDEBUG
      const OccupancyGuard guard{caps.Get("ConsumeGets", 1)};
    #endif
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
  static std::string GetName() { return "PendingDuct"; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("buffer_t buffer", buffer[0]) << std::endl;
    ss << format_member("pending_t pending_gets", pending_gets);
    ss << format_member("pending_t get_position", get_position);
    return ss.str();
  }


};

} // namespace internal
} // namespace uit
