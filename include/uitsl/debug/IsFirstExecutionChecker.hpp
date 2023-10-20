#pragma once
#ifndef UITSL_DEBUG_ISFIRSTEXECUTIONCHECKER_HPP_INCLUDE
#define UITSL_DEBUG_ISFIRSTEXECUTIONCHECKER_HPP_INCLUDE

#include <atomic>
#include <cassert>
#include <stddef.h>

namespace uitsl {

class IsFirstExecutionChecker {

  std::atomic<size_t> flag{false};

public:

  IsFirstExecutionChecker() = default;

  IsFirstExecutionChecker(const uitsl::IsFirstExecutionChecker& other)
  : flag(other.flag.load())
  { ; }

  void Check() { assert(0 == flag++); }

};

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_ISFIRSTEXECUTIONCHECKER_HPP_INCLUDE
