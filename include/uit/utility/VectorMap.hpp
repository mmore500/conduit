#pragma once

#include "../../../third-party/Empirical/source/base/vector.h"

namespace uit {

template<typename Key, typename T>
class VectorMap {

  emp::vector<T> data;

public:

  T& operator[](const Key i) {
    if (i >= data.size()) data.resize(i + 1);
    return at(i);
  }

  T& at(const Key i) { return data[i]; }

  const T& at(const Key i) const { return data[i]; }

};

} // namespace uit
