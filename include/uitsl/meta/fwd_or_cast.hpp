#pragma once
#ifndef UITSL_META_FWD_OR_CAST_HPP_INCLUDE
#define UITSL_META_FWD_OR_CAST_HPP_INCLUDE

#include <utility>

namespace uitsl {

template<template<typename> typename TEST, typename CAST_TO, typename ARG>
decltype(auto) fwd_or_cast(ARG&& arg) {
  if constexpr (TEST<ARG>::value) return std::forward<ARG>(arg);
  else return static_cast<CAST_TO>(arg);
}

} // namespace uitsl

#endif // #ifndef UITSL_META_FWD_OR_CAST_HPP_INCLUDE
