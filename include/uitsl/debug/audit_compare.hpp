#pragma once
#ifndef UITSL_DEBUG_AUDIT_COMPARE_HPP_INCLUDE
#define UITSL_DEBUG_AUDIT_COMPARE_HPP_INCLUDE

#include "safe_compare.hpp"

namespace uitsl {

template <typename I, typename J>
inline bool audit_greater(const I x, const J y) {
  #ifndef NDEBUG
    return uitsl::safe_greater( x , y );
  #else
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wsign-compare"
    return x > y;
    #pragma GCC diagnostic pop
  #endif
}

template <typename I, typename J>
inline bool audit_less(const I x, const J y) {
  #ifndef NDEBUG
    return uitsl::safe_less( x , y );
  #else
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wsign-compare"
    return x < y;
    #pragma GCC diagnostic pop
  #endif
}

template <typename I, typename J>
inline bool audit_leq(const I x, const J y) {
  #ifndef NDEBUG
    return uitsl::safe_leq( x , y );
  #else
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wsign-compare"
    return x <= y;
    #pragma GCC diagnostic pop
  #endif
}

template <typename I, typename J>
inline bool audit_geq(const I x, const J y) {
  #ifndef NDEBUG
    return uitsl::safe_geq( x , y );
  #else
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wsign-compare"
    return x >= y;
    #pragma GCC diagnostic pop
  #endif
}

template <typename I, typename J>
inline bool audit_equal(const I x, const J y) {
  #ifndef NDEBUG
    return uitsl::safe_equal( x , y );
  #else
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wsign-compare"
    return x == y;
    #pragma GCC diagnostic pop
  #endif
}

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_AUDIT_COMPARE_HPP_INCLUDE
