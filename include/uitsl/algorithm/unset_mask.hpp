#pragma once
#ifndef UITSL_ALGORITHM_UNSET_MASK_HPP_INCLUDE
#define UITSL_ALGORITHM_UNSET_MASK_HPP_INCLUDE

#include "../../../third-party/Empirical/source/tools/BitSet.h"


namespace uitsl {

template<typename T>
T unset_mask(const T& target, const T& mask) {

  return ~(
    (~target) | mask
  );

}

} // namespace uitsl

#endif // #ifndef UITSL_ALGORITHM_UNSET_MASK_HPP_INCLUDE
