#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_IMPL_ROUNDTRIPCOUNTPACKET_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_IMPL_ROUNDTRIPCOUNTPACKET_HPP_INCLUDE

#include <cstddef>
#include <tuple>

namespace uit {
namespace impl {

template<typename T>
struct RoundTripCountPacket {

  size_t round_trip_count;
  T data;

  template<class Archive>
  void serialize(Archive& archive) { archive( round_trip_count, data ); }

  bool operator==(const RoundTripCountPacket& other) const {
    return std::tuple{
      round_trip_count,
      data,
    } == std::tuple{
      other.round_trip_count,
      other.data
    };
  }

};

} // namespace impl
} // namespace uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_IMPL_ROUNDTRIPCOUNTPACKET_HPP_INCLUDE
