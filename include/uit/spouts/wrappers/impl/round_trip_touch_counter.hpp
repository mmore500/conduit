#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_IMPL_ROUND_TRIP_TOUCH_COUNTER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_IMPL_ROUND_TRIP_TOUCH_COUNTER_HPP_INCLUDE

#include <tuple>
#include <unordered_map>

#include "RoundTripCounterAddr.hpp"

namespace uit {
namespace impl {

// (mesh id, min node id, max node id) -> round trip touch count
using round_trip_touch_addr_t = uit::impl::RoundTripCounterAddr;
using round_trip_touch_counter_t = std::unordered_map<
  round_trip_touch_addr_t,
  size_t
>;

// all set up (adding addrs) performed by uit::Mesh
inline static round_trip_touch_counter_t round_trip_touch_counter{};

} // namespace impl
} // namespace uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_IMPL_ROUND_TRIP_TOUCH_COUNTER_HPP_INCLUDE
