#pragma once

#include <utility>
#include <memory>
#include <stddef.h>

#include "Duct.hpp"
#include "Inlet.hpp"
#include "Outlet.hpp"

namespace uit {

template<typename ImplSpec>
class Conduit {

  using duct_t = uit::Duct<ImplSpec>;
  std::shared_ptr<duct_t> duct{ std::make_shared<duct_t>() };

  uit::Inlet<ImplSpec> inlet{ duct };
  uit::Outlet<ImplSpec> outlet{ duct };

public:

  // for structured bindings
  template <size_t N>
  decltype(auto) get() const {
      // parens needed to get reference?
      if constexpr (N == 0) return (inlet);
      else if constexpr (N == 1) return (outlet);
  }

  uit::Inlet<ImplSpec> & GetInlet() { return inlet; }

  uit::Outlet<ImplSpec> & GetOutlet() { return outlet; }

  // TODO implicit conversion operators?


};

}

// for structured bindings
namespace std {

  template<typename ImplSpec>
  struct tuple_size<uit::Conduit<ImplSpec>>
    : std::integral_constant<size_t, 2>{};

  template<typename ImplSpec, size_t N>
  struct tuple_element<N, uit::Conduit<ImplSpec>> {

    using type = decltype(
      std::declval<uit::Conduit<ImplSpec>>().template get<N>()
    );
  };

}
