#pragma once
#ifndef UITSL_POLYFILL_IMPL_REMOVE_CVREF_HPP_INCLUDE
#define UITSL_POLYFILL_IMPL_REMOVE_CVREF_HPP_INCLUDE

#include <type_traits>

namespace std {

// adapted from https://en.cppreference.com/w/cpp/types/remove_cvref
template< class T >
struct remove_cvref {
  typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};

template< class T >
using remove_cvref_t = typename remove_cvref<T>::type;

} // namespace std

#endif // #ifndef UITSL_POLYFILL_IMPL_REMOVE_CVREF_HPP_INCLUDE
