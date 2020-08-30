#pragma once

#include <utility>

#include "audited_routine_aliases.hpp"

namespace uit {

struct IsendFunctor {

  template<typename... Args>
  void operator()(Args&&... args) { UIT_Isend(std::forward<Args>(args)...); }

};

} // namespace uit
