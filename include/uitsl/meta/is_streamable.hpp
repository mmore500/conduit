#pragma once
#ifndef UITSL_META_IS_STREAMABLE_HPP_INCLUDE
#define UITSL_META_IS_STREAMABLE_HPP_INCLUDE

#include <iostream>
#include <type_traits>

namespace uitsl {

// adapted from https://stackoverflow.com/a/22760197
template <typename T>
class is_streamable
{
    template <typename U> // must be template to get SFINAE fall-through...
    static auto test(const U* u) -> decltype(std::cout << *u);

    static auto test(...)        -> std::false_type;

 public:
    enum { value = !std::is_same_v<decltype(test((T*)0)), std::false_type> };
};

} // namespace uitsl

#endif // #ifndef UITSL_META_IS_STREAMABLE_HPP_INCLUDE
