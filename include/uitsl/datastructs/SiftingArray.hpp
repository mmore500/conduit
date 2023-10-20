#pragma once
#ifndef UITSL_DATASTRUCTS_SIFTINGARRAY_HPP_INCLUDE
#define UITSL_DATASTRUCTS_SIFTINGARRAY_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <utility>

namespace uitsl {

template<typename T, size_t N>
class SiftingArray {

  std::array<T, N> data;

  size_t num_items{};

public:

  SiftingArray() { ; }

  explicit SiftingArray(const T& t) {
    data.fill(t);
    num_items = N;
  }

  void SiftByValue(const std::function<bool(const T& val)>& predicate) {

    const auto partition_iterator = std::partition(
      std::begin(data),
      std::begin(data) + num_items,
      predicate
    );

    num_items = std::distance(
      std::begin(data),
      partition_iterator
    );

  }

  void SiftByIndex(const std::function<bool(size_t)>& predicate) {

    SiftByValue( [this, &predicate](const T& val){ return predicate(
      std::distance( std::as_const(data).data(), &val )
    ); } );

  }

  size_t GetSize() const {
    return num_items;
  }

  T* GetData() { return data.data(); }

  const T* GetData() const { return data.data(); }

  T& Get(const size_t i) { assert( i < num_items ); return data[i]; }

  const T& Get(const size_t i) const {
    assert( i < num_items );
    return data[i];
  }

  T& Front() { assert( num_items ); return Get(0); }

  const T& Front() const { assert( num_items ); return Get(0); }

  T& Back() { assert( num_items ); return Get(num_items - 1); }

  const T& Back() const { assert( num_items ); return Get(num_items - 1); }

  void PushBack(const T& val=T{}) {
    assert( num_items < N );
    ++num_items;
    Get(num_items - 1) = val;
  };

  template<typename P>
  void PushBack(P&& val) {
    assert( num_items < N );
    ++num_items;
    Get(num_items - 1) = std::forward<P>(val);
  }

  bool PopBack() {
    if ( GetSize() ) {
      --num_items;
      return true;
    } else return false;
  }

  T* begin() { return data.data(); }

  const T* begin() const { return data.data(); }

  T* end() { return data.data() + num_items; }

  const T* end() const { return data.data() + num_items; }

};


}

#endif // #ifndef UITSL_DATASTRUCTS_SIFTINGARRAY_HPP_INCLUDE
