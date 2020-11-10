#pragma once
#ifndef UITSL_ALGORITHM_CLAMP_CAST_HPP_INCLUDE
#define UITSL_ALGORITHM_CLAMP_CAST_HPP_INCLUDE

#include <algorithm>
#include <cmath>
#include <limits>

#include "nan_to_zero.hpp"

namespace uitsl {

template<typename To, typename From>
To clamp_cast( const From from ) {

  // ideally, we'd use std::nexttoward but unfortunately it's not constexpr
  constexpr double lower_bound =
    static_cast<double>( std::numeric_limits<To>::min() )
    + std::numeric_limits<double>::epsilon()
  ;

  constexpr double upper_bound =
    static_cast<double>( std::numeric_limits<To>::max() )
    - std::numeric_limits<double>::epsilon()
  ;

  return static_cast<To>( std::clamp(
    static_cast<double>( uitsl::nan_to_zero( from ) ),
    lower_bound,
    upper_bound
  ) );
}

} // namespace uitsl

#endif // #ifndef UITSL_ALGORITHM_CLAMP_CAST_HPP_INCLUDE
