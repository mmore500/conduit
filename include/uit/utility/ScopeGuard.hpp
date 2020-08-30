#pragma once

#include <functional>

namespace uit {

class ScopeGuard {

  std::function<void()> on_destruct;

public:
  template <typename OnConstruct, typename OnDestruct=std::function<void()>>
  ScopeGuard(
    OnConstruct&& on_construct=[](){},
    OnDestruct&& on_destruct_=[](){}
  ) : on_destruct(on_destruct_) { on_construct(); }

  ~ScopeGuard() { on_destruct(); }

};

} // namespace uit
