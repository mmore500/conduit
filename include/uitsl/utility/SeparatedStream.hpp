#pragma once
#ifndef UITSL_UTILITY_SEPARATEDSTREAM_HPP_INCLUDE
#define UITSL_UTILITY_SEPARATEDSTREAM_HPP_INCLUDE

#include <iostream>
#include <string>
#include <utility>

namespace uitsl {

// adapted from https://stackoverflow.com/a/30073885
class SeparatedStream {

  std::ostream& _stream;
  std::string _sep;
  bool _first;

public:

  SeparatedStream(std::ostream &stream, std::string sep)
  : _stream(stream), _sep(std::move(sep)), _first(true) {}

  template <class Rhs>
  SeparatedStream &operator <<(Rhs &&rhs) {
    if (_first) _first = false;
    else _stream << _sep;
    _stream << std::forward<Rhs>(rhs);
    return *this;
  }

  SeparatedStream& operator<<(std::ostream &(*manip)(std::ostream&)) {
    manip(_stream);
    return *this;
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_SEPARATEDSTREAM_HPP_INCLUDE
