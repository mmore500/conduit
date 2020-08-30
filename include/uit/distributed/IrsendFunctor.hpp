#pragma once

#include <utility>

#include "audited_routine_aliases.hpp"

namespace uit {

struct IrsendFunctor {

  template<typename... Args>
  void operator()(Args&&... args) { UIT_Irsend(std::forward<Args>(args)...); }

};

} //namespace uit
