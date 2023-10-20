#pragma once
#ifndef UITSL_UTILITY_SETSEPARATOR_HPP_INCLUDE
#define UITSL_UTILITY_SETSEPARATOR_HPP_INCLUDE

#include <iostream>
#include <string>
#include <utility>

#include "SeparatedStream.hpp"

namespace uitsl {

// adapted from https://stackoverflow.com/a/30073885
struct SetSeparator {

  std::string sep;

  SetSeparator(const std::string& sep) : sep(sep) {}

};

uitsl::SeparatedStream operator<<(
  std::ostream& stream,
  uitsl::SetSeparator wsep
) {
  return uitsl::SeparatedStream(stream, std::move(wsep.sep));
}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_SETSEPARATOR_HPP_INCLUDE
