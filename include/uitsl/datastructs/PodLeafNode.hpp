#pragma once
#ifndef UITSL_DATASTRUCTS_PODLEAFNODE_HPP_INCLUDE
#define UITSL_DATASTRUCTS_PODLEAFNODE_HPP_INCLUDE

#include "../../../third-party/Empirical/source/base/array.h"

namespace uitsl {

template<typename T, size_t N=1>
class PodLeafNode {

  emp::array<T, N> data;

public:

  static constexpr size_t GetSize() { return N; }

  /*
   * Access value by index at compile time.
   */
  template<size_t Query>
  constexpr T& Get() { return data.at(Query); }

  /*
   * Access value by index at compile time.
   */
  template<size_t Query>
  constexpr const T& Get() const { return data.at(Query); }

  /*
   * Access value by index at runtime.
   */
  constexpr T& Get(const size_t query=0) { return data.at(query); }

  /*
   * Access value by index at runtime.
   */
  constexpr const T& Get(const size_t query=0) const { return data.at(query); }

  /*
   * Access value.
   * Template oncluded for compatibility with other components
   */
  template<typename Query=T>
  constexpr T& Get() {
    static_assert( std::is_same<Query, T>::value );
    return data.at(0);
  }

  /*
   * Access value.
   * Template oncluded for compatibility with other components
   */
  template<typename Query=T>
  constexpr const T& Get() const {
    static_assert( std::is_same<Query, T>::value );
    return data.at(0);
  }


  /*
   * Does leaf array contain type?
   */
  template<typename Query>
  static constexpr bool HasType() { return std::is_same<T, Query>(); }


  /*
   * Equality operator overload.
   */
  bool operator==(const PodLeafNode& other) const {
    return data == other.data;
  }

  /*
   * Set data to value-initialized state.
   */
  void Reset() { data.fill( T{} ); }

};

} // namespace uitsl

#endif // #ifndef UITSL_DATASTRUCTS_PODLEAFNODE_HPP_INCLUDE
