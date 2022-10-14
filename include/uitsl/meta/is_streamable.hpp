#pragma once
#ifndef UITSL_META_IS_STREAMABLE_HPP_INCLUDE
#define UITSL_META_IS_STREAMABLE_HPP_INCLUDE

#include <iostream>
#include <type_traits>

namespace uitsl {

// adapted from https://stackoverflow.com/a/22760197
template <typename T, class = void>
struct is_streamable : std::false_type { };

template <typename T>
struct is_streamable<T, std::void_t<decltype(std::cout << *(T*)0)>>
  : std::true_type { };

} // namespace uitsl

#endif // #ifndef UITSL_META_IS_STREAMABLE_HPP_INCLUDE
