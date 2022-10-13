#pragma once
#ifndef UITSL_PARALLEL_THREADTEAM_HPP_INCLUDE
#define UITSL_PARALLEL_THREADTEAM_HPP_INCLUDE

#include <algorithm>
#include <stddef.h>
#include <thread>
#include <utility>
#include <vector>

#include "../polyfill/erase_if.hpp"

#include "_TryJoinableThread.hpp"

namespace uitsl {

class ThreadTeam {

  std::vector<internal::TryJoinableThread> workers;

public:

  template <typename... Args>
  void Add(Args&&... args) {
    workers.emplace_back(std::forward<Args>(args)...);
  }

  void Join() {
    std::for_each(
      std::begin(workers),
      std::end(workers),
      [](auto & worker){ worker.Join(); }
    );
    workers.clear();
  }

  bool TryJoin() {
    std::erase_if(
      workers,
      [](auto& worker){ return worker.TryJoin(); }
    );
    return workers.size() == 0;
  }

  size_t Size() const { return workers.size(); }

};

} // namespace uitsl

#endif // #ifndef UITSL_PARALLEL_THREADTEAM_HPP_INCLUDE
