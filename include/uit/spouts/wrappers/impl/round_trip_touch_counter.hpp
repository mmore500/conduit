#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_IMPL_ROUND_TRIP_TOUCH_COUNTER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_IMPL_ROUND_TRIP_TOUCH_COUNTER_HPP_INCLUDE

#include <tuple>
#include <unordered_map>

#include "../../../../../third-party/Empirical/include/emp/datastructs/tuple_utils.hpp"

namespace uit {
namespace impl {

// (mesh id, min node id, max node id) -> round trip touch count
using round_trip_touch_addr_t = std::tuple<size_t, size_t, size_t>;
using round_trip_touch_counter_t = std::unordered_map<
  round_trip_touch_addr_t,
  size_t,
  emp::TupleHash<size_t, size_t, size_t>
>;
// set up done by mesh
inline static round_trip_touch_counter_t round_trip_touch_counter{};

} // namespace impl
} // namespace uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_IMPL_ROUND_TRIP_TOUCH_COUNTER_HPP_INCLUDE
