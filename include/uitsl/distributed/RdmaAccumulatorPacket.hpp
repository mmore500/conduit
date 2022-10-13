#pragma once
#ifndef UITSL_DISTRIBUTED_RDMAACCUMULATORPACKET_HPP_INCLUDE
#define UITSL_DISTRIBUTED_RDMAACCUMULATORPACKET_HPP_INCLUDE

#include <algorithm>
#include <iterator>

#include "../utility/NamedArrayElement.hpp"

namespace uitsl {

template<typename T>
struct RdmaAccumulatorPacket {

  union {

    T m_array[2];

    NamedArrayElement<0, T> data;
    NamedArrayElement<1, T> epoch;

  };


  RdmaAccumulatorPacket() {
    std::fill( std::begin(m_array), std::end(m_array), T{} );
  }

  RdmaAccumulatorPacket(const T& data_, const T& epoch_) {
    data = data_;
    epoch = epoch_;
  }


};

} // namespace uitsl

#endif // #ifndef UITSL_DISTRIBUTED_RDMAACCUMULATORPACKET_HPP_INCLUDE
