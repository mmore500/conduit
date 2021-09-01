#pragma once
#ifndef UITSL_DEBUG_NOTIMPLEMENTEDEXCEPTION_HPP_INCLUDE
#define UITSL_DEBUG_NOTIMPLEMENTEDEXCEPTION_HPP_INCLUDE

#include <stdexcept>

namespace uitsl {

// adapted from https://stackoverflow.com/a/36851059
class NotImplementedException : public std::logic_error {

public:
  NotImplementedException()
  : std::logic_error("Function not yet implemented.")
  { }

};

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_NOTIMPLEMENTEDEXCEPTION_HPP_INCLUDE
