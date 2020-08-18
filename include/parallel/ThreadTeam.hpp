#pragma once

#include <algorithm>
#include <stddef.h>
#include <thread>
#include <utility>

#include "../../third-party/Empirical/source/base/vector.h"

namespace uit {

class ThreadTeam {

  emp::vector<std::thread> workers;

public:

  template <typename... Args>
  void Add(Args&&... args) {
    workers.emplace_back(std::forward<Args>(args)...);
  }

  void Join() {
    std::for_each(
      std::begin(workers),
      std::end(workers),
      [](auto & worker){ worker.join(); }
    );
    workers.clear();
  }

  size_t Size() const { return workers.size(); }

};

} // namespace uit
