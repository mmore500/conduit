#pragma once

#include "../../../third-party/Empirical/source/base/vector.h"

namespace uit {

template<typename T>
class VectorMap {

  emp::vector<T> data;

public:

  T& operator[](const size_t i) {
    if (i >= data.size()) data.resize(i + 1);
    return at(i);
  }

  T& at(const size_t i) { return data[i]; }

  const T& at(const size_t i) const { return data[i]; }

};

} // namespace uit
