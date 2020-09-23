#pragma once

#include <type_traits>

namespace uitsl {

// adapted from https://en.cppreference.com/w/cpp/types/decay
template <typename T, typename U>
struct decay_equiv : std::is_same<typename std::decay<T>::type, U>::type
{};

} // namespace uitsl
