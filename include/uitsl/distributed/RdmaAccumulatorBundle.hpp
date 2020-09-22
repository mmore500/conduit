#pragma once

#include <algorithm>

#include "../../../third-party/Empirical/source/polyfill/span.h"
#include "../../../third-party/Empirical/source/base/vector.h"

#include "../utility/NamedArrayElement.hpp"

namespace uitsl {

template<typename T>
class RdmaAccumulatorBundle {

  size_t data_size;
  mutable emp::vector<T> buff;

public:

  T* data() { return buff.data(); }

  const T* data() const { return buff.data(); }

  size_t size() const { return buff.size(); }

  size_t byte_size() const { return sizeof(T) * size(); }

  emp::vector<T>& GetData() {
    if (buff.size() > data_size) buff.pop_back();
    return buff;
  }

  const emp::vector<T>& GetData() const {
    if (buff.size() > data_size) buff.pop_back();
    return buff;
    }

  void Reset() {
    buff.resize(data_size + 1); // +1 for epoch
    std::fill( std::begin(buff), std::end(buff), T{} );
  }

  T GetEpoch() const {
    emp_assert( buff.size() == data_size + 1 );
    return buff.back();
  }

  void BumpEpoch() {
    emp_assert( buff.size() == data_size + 1 );
    ++buff.back();
  }

  void BumpData(const emp::vector<T>& bumps) {
    emp_assert( bumps.size() == data_size );
    emp_assert( buff.size() == data_size + 1 );
    std::transform(
      std::begin(bumps),
      std::end(bumps),
      std::begin(buff),
      std::begin(buff),
      std::plus<int>{}
    );
  }

  RdmaAccumulatorBundle(const size_t data_size_)
  : data_size( data_size_ )
  , buff( data_size + 1 )
  { }

  RdmaAccumulatorBundle(const emp::vector<T>& data)
  : data_size( data.size() )
  , buff( data )
  { buff.emplace_back(); }

  RdmaAccumulatorBundle(emp::vector<T>&& data)
  : data_size( data.size() )
  , buff( std::move(data) )
  { buff.emplace_back(); }

  RdmaAccumulatorBundle(RdmaAccumulatorBundle &&) = default;
  RdmaAccumulatorBundle& operator=(RdmaAccumulatorBundle &&) = default;

};

} // namespace uitsl
