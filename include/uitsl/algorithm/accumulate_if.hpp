#pragma once
#ifndef UITSL_ALGORITHM_ACCUMULATE_IF_HPP_INCLUDE
#define UITSL_ALGORITHM_ACCUMULATE_IF_HPP_INCLUDE

namespace uitsl {

// adapted from https://stackoverflow.com/a/20028984
template<
  typename InputIterator,
  typename AccumulateType,
  typename BinaryOperation,
  typename Predicate
> const AccumulateType accumulate_if(
  InputIterator first,
  const InputIterator last,
  AccumulateType init,
  BinaryOperation&& binary_op,
  Predicate&& predicate
) {
  for (; first != last; ++first) {
    if (predicate(*first)) {
      init = binary_op(init, *first);
    }
  }
  return init;
}

} // namespace uitsl

#endif // #ifndef UITSL_ALGORITHM_ACCUMULATE_IF_HPP_INCLUDE
