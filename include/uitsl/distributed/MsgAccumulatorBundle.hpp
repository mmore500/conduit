#pragma once

#include <algorithm>

#include "../../../third-party/Empirical/source/polyfill/span.h"
#include "../../../third-party/Empirical/source/base/vector.h"

#include "../math/divide_utils.hpp"

namespace uitsl {

template<typename T>
class MsgAccumulatorBundle {

  constexpr static size_t epoch_embed_items {
     uitsl::div_ceil(sizeof(size_t), sizeof(T))
  };

  size_t data_size;
  // holds data as first data_size
  // then epoch_embed_items count at the end
  // for epoch as a size_t
  mutable emp::vector<T> buff;

  size_t const size() const { return buff.size(); }

public:

  T* data() { return buff.data(); }

  const T* data() const { return buff.data(); }

  size_t byte_size() const {
    emp_assert( buff.size() == data_size + epoch_embed_items );
    return sizeof(T) * data_size + sizeof(size_t);
  }

  emp::vector<T>& GetData() {
    while (buff.size() > data_size) buff.pop_back();
    return buff;
  }

  const emp::vector<T>& GetData() const {
    while (buff.size() > data_size) buff.pop_back();
    return buff;
    }

  void Reset() {
    buff.resize(data_size + epoch_embed_items);
    std::fill(
      std::begin(buff),
      std::begin(buff) + data_size,
      T{}
    );
    SetEpoch( size_t{} );
  }

  size_t GetEpoch() const {
    emp_assert( buff.size() == data_size + epoch_embed_items );
    size_t res;
    std::memcpy(
      &res,
      buff.data() + data_size,
      sizeof(size_t)
    );
    return res;
  }

  void SetEpoch(const size_t val) {
    emp_assert( buff.size() == data_size + epoch_embed_items );
    std::memcpy(
      buff.data() + data_size,
      &val,
      sizeof(size_t)
    );
  }

  void BumpEpoch() {
    emp_assert( buff.size() == data_size + epoch_embed_items );
    SetEpoch( GetEpoch() + 1 );
  }

  void BumpData(const emp::vector<T>& bumps) {
    emp_assert( bumps.size() == data_size );
    emp_assert( buff.size() == data_size + epoch_embed_items );
    std::transform(
      std::begin(bumps),
      std::end(bumps),
      std::begin(buff),
      std::begin(buff),
      std::plus<T>{}
    );
  }

  MsgAccumulatorBundle(const size_t data_size_)
  : data_size( data_size_ )
  { Reset(); }

  MsgAccumulatorBundle(const emp::vector<T>& data)
  : data_size( data.size() )
  , buff( data )
  {
    buff.resize( data_size + epoch_embed_items);
    SetEpoch( size_t{} );
  }

  MsgAccumulatorBundle(emp::vector<T>&& data)
  : data_size( data.size() )
  , buff( std::move(data) )
  {
    buff.resize( data_size + epoch_embed_items);
    SetEpoch( size_t{} );
  }

  MsgAccumulatorBundle(MsgAccumulatorBundle &&) = default;
  MsgAccumulatorBundle& operator=(MsgAccumulatorBundle &&) = default;

  MsgAccumulatorBundle& operator+=(const MsgAccumulatorBundle& rhs) {
    emp_assert( size() == rhs.size() );
    std::transform(
      rhs.buff.data(),
      rhs.buff.data() + data_size,
      buff.data(),
      buff.data(),
      std::plus<T>{}
    );
    SetEpoch( rhs.GetEpoch() + GetEpoch() );
    return *this;
  }

};

} // namespace uitsl
