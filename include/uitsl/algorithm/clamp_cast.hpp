#pragma once
#ifndef UITSL_ALGORITHM_CLAMP_CAST_HPP_INCLUDE
#define UITSL_ALGORITHM_CLAMP_CAST_HPP_INCLUDE

#include <algorithm>
#include <limits>

#include "nan_to_zero.hpp"

namespace uitsl {

template<typename To, typename From>
To clamp_cast( const From from ) {
  return static_cast<To>( std::clamp(
    static_cast<double>( uitsl::nan_to_zero( from ) ),
    static_cast<double>( std::numeric_limits<To>::min() ),
    static_cast<double>( std::numeric_limits<To>::max() )
  ) );
}

} // namespace uitsl

#endif // #ifndef UITSL_ALGORITHM_CLAMP_CAST_HPP_INCLUDE
