#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_IMPL_ROUNDTRIPCOUNTERADDR_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_IMPL_ROUNDTRIPCOUNTERADDR_HPP_INCLUDE

#include <algorithm>
#include <tuple>

#include "../../../../../third-party/Empirical/include/emp/datastructs/hash_utils.hpp"

namespace uit {
namespace impl {

struct RoundTripCounterAddr {

  size_t mesh_id;
  size_t smallest_node_id;
  size_t largest_node_id;

  RoundTripCounterAddr(
    const size_t mesh_id_,
    const size_t first_node_id,
    const size_t second_node_id
  ) : mesh_id( mesh_id_ )
  , smallest_node_id( std::min( first_node_id, second_node_id ) )
  , largest_node_id( std::max( first_node_id, second_node_id ) )
  { }

  bool operator==(const RoundTripCounterAddr& other) const {
    return std::tuple{
      mesh_id,
      smallest_node_id,
      largest_node_id
    } ==  std::tuple{
      other.mesh_id,
      other.smallest_node_id,
      other.largest_node_id
    };
  }

};

} // namespace impl
} // namespace uit

namespace std {

  template <>
  struct hash<uit::impl::RoundTripCounterAddr> {
    size_t operator()(const uit::impl::RoundTripCounterAddr& addr) const {
      return emp::CombineHash(
        addr.mesh_id,
        addr.smallest_node_id,
        addr.largest_node_id
      );
    }
  };

} // namespace std

#endif // #ifndef UIT_SPOUTS_WRAPPERS_IMPL_ROUNDTRIPCOUNTERADDR_HPP_INCLUDE
