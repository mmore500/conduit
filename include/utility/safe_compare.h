#pragma once

#include <limits>
#include <type_traits>

#include "tools/math.h"

namespace internal {

  // adapted from https://www.jwwalker.com/pages/safe-compare.html
  template <typename T>
  struct NumType {
    enum {
      Code = (
        (std::numeric_limits<T>::is_integer? 0 : 2)
        + (std::numeric_limits<T>::is_signed? 1 : 0)
      )
    };
  };

  template <typename I, typename J, int typeCodeI, int typeCodeJ>
  struct SafeImp {
    static inline bool Less(I x, J y) {
      return x < y;
    }
  };

  template <typename I, typename J>
  struct SafeImp<I, J, 1, 0> {
    static inline bool Less(I x, J y) {
      return (x < 0) || (
        static_cast<typename std::make_unsigned<I>::type>(x) < y
      );
    }
  };

  template <typename I, typename J>
  struct SafeImp<I, J, 0, 1> {
    static inline bool Less(I x, J y) {
      return (y >= 0) && (
        x < static_cast<typename std::make_unsigned<J>::type>(y)
      );
    }
  };

}

template <typename I, typename J>
inline bool safe_less(I x, J y) {
  return internal::SafeImp<
    I,
    J,
    internal::NumType<I>::Code,
    internal::NumType<J>::Code
  >::Less(x, y);
}

template <typename I, typename J>
inline bool safe_greater(I x, J y) {
  return safe_less<J, I>(y, x);
}

template <typename I, typename J>
inline bool safe_equal(I x, J y) {
  return !safe_less<I, J>(x, y) && !safe_greater<I, J>(x, y);
}
