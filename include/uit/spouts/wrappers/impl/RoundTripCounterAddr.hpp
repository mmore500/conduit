#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_IMPL_ROUNDTRIPCOUNTERADDR_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_IMPL_ROUNDTRIPCOUNTERADDR_HPP_INCLUDE

#include <tuple>

#include "../../../../uit_emp/datastructs/hash_utils.hpp"
#include "../../../../uit_emp/meta/meta.hpp"

namespace uit {
namespace impl {

struct RoundTripCounterAddr {

  size_t mesh_id;
  size_t this_spout_node_id;
  size_t partner_spout_node_id;

  bool operator==(const RoundTripCounterAddr& other) const {
    return std::tuple{
      mesh_id,
      this_spout_node_id,
      partner_spout_node_id
    } ==  std::tuple{
      other.mesh_id,
      other.this_spout_node_id,
      other.partner_spout_node_id
    };
  }

};

} // namespace impl
} // namespace uit

namespace std {

  template <>
  struct hash<uit::impl::RoundTripCounterAddr> {
    size_t operator()(const uit::impl::RoundTripCounterAddr& addr) const {
      return uit_emp::CombineHash(
        addr.mesh_id,
        addr.this_spout_node_id,
        addr.partner_spout_node_id
      );
    }
  };

} // namespace std

#endif // #ifndef UIT_SPOUTS_WRAPPERS_IMPL_ROUNDTRIPCOUNTERADDR_HPP_INCLUDE
