#pragma once

// TODO refactor this to preserve levelization
#include "../conduit/config.hpp"

namespace uit {

template <typename T>
class alignas(CACHE_LINE_SIZE) AlignedInherit : public T {

public:

  std::string ToString() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

};

} // namespace uit
