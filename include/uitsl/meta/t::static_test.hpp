#pragma once

#include <type_traits>

#define uitsl_t_message "T is not a trivial type"

namespace uitsl {
namespace t { // trivial type

template<typename T>
class StaticTest : public std::is_trivial<T> {};

template<typename T>
constexpr bool static_test() { return StaticTest<T>::value; }

} // namespace t
} // namespace uitsl
