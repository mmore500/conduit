#pragma once

#include <algorithm>
#include <stddef.h>
#include <utility>

#include "../../../third-party/Empirical/source/base/array.h"

#include "CircularIndex.hpp"

namespace uit {

template<typename T, size_t N>
class RingBuffer {

  emp::array<T, N> buffer{};
  uit::CircularIndex<N> tail{};
  size_t num_items{};

public:

  size_t GetSize() const { return num_items; }

  bool PushHead(const T& t=T{}) {
    if (GetSize() == N) return false;
    else {
      Get(num_items) = t;
      ++num_items;
      return true;
    }
  }

  template<typename P>
  bool PushHead(P&& p) {
    if (GetSize() == N) return false;
    else {
      Get(num_items) = std::forward<P>( p );
      ++num_items;
      return true;
    }
  }

  size_t PopTail(const size_t num_requested=1) {
    const size_t num_popped = std::min(num_requested, GetSize());
    tail += num_popped;
    num_items -= num_popped;
    return num_popped;
  }

  void Fill(const T& t=T{}) { while( PushHead(t) ); }

  const T& Get(const size_t i) const { return buffer[tail + i]; }

  T& Get(const size_t i) { return buffer[tail + i]; }

  T& GetHead() {
    emp_assert( GetSize() );
    return Get( GetSize() - 1 );
  }

  const T& GetHead() const {
    emp_assert( GetSize() );
    return Get( GetSize() - 1 );
  }

  const T& GetTail() const { return Get(0); }

};

} // namespace uit
