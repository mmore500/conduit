#pragma once

#include "../../../uitsl/meta/IdentityWrapper.hpp"

namespace uit {

template<typename T_>
class TrivialSpoutWrapper {

public:
  using T = T_;

  template<typename Inlet>
  using inlet_wrapper_t = uitsl::IdentityWrapper<Inlet>;

  template<typename Outlet>
  using outlet_wrapper_t = uitsl::IdentityWrapper<Outlet>;

};



} // namespace uit
