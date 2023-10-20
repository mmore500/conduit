#pragma once
#ifndef UITSL_DISTRIBUTED_RDMAACCUMULATORBUNDLE_HPP_INCLUDE
#define UITSL_DISTRIBUTED_RDMAACCUMULATORBUNDLE_HPP_INCLUDE

#include <algorithm>
#include <cassert>
#include <cstring>
#include <vector>

#include "../utility/NamedArrayElement.hpp"

namespace uitsl {

template<typename T>
class RdmaAccumulatorBundle {

  size_t data_size;
  // holds data within size then epoch past the end
  // within reserved capacity
  std::vector<T> buff;

public:

  T* data() { return buff.data(); }

  const T* data() const { return buff.data(); }

  size_t byte_size() const {
    assert( buff.size() == data_size );
    assert( buff.capacity() == data_size + 1);
    return sizeof(T) * (data_size + 1);
  }

  std::vector<T>& GetData() { return buff; }

  const std::vector<T>& GetData() const { return buff; }

  void Reset() {
    buff.reserve( data_size + 1 ); // +1 for epoch
    buff.resize(data_size);
    std::fill( std::begin(buff), std::end(buff), T{} );
    SetEpoch( T{} );
  }

  T GetEpoch() const {
    assert( buff.size() == data_size );
    assert( buff.capacity() == data_size + 1);
    T res;
    std::memcpy(
      &res,
      buff.data() + data_size,
      sizeof(T)
    );
    return res;
  }

  void SetEpoch(const T val) {
    assert( buff.size() == data_size );
    assert( buff.capacity() == data_size + 1);
    std::memcpy(
      buff.data() + data_size,
      &val,
      sizeof(T)
    );
  }

  void BumpEpoch(const T amt=1) { SetEpoch( GetEpoch() + amt ); }


  void BumpData(const std::vector<T>& bumps) {
    assert( bumps.size() == data_size );
    assert( buff.size() == data_size );
    std::transform(
      std::begin(bumps),
      std::end(bumps),
      std::begin(buff),
      std::begin(buff),
      std::plus<T>{}
    );
  }

  RdmaAccumulatorBundle(const size_t data_size_)
  : data_size( data_size_ )
  { Reset(); }

  RdmaAccumulatorBundle(const std::vector<T>& data)
  : data_size( data.size() )
  , buff( data )
  { buff.reserve( data_size + 1 ); SetEpoch( T{} ); }

  RdmaAccumulatorBundle(std::vector<T>&& data)
  : data_size( data.size() )
  , buff( std::move(data) )
  { buff.reserve( data_size + 1 ); SetEpoch( T{} ); }

  RdmaAccumulatorBundle(RdmaAccumulatorBundle &&) = default;
  RdmaAccumulatorBundle& operator=(RdmaAccumulatorBundle &&) = default;

};

} // namespace uitsl

#endif // #ifndef UITSL_DISTRIBUTED_RDMAACCUMULATORBUNDLE_HPP_INCLUDE
