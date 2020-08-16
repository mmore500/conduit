#pragma once

#include <utility>
#include <memory>
#include <stddef.h>

#include "Duct.hpp"
#include "Outlet.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class Source {

  uit::Outlet<ImplSpec> outlet{
    std::make_shared<uit::Duct<ImplSpec>>()
  };

public:

  // for structured bindings
  template <size_t N>
  decltype(auto) get() const {
      // parens needed to get reference?
      if constexpr (N == 0) return (outlet);
  }

  uit::Outlet<ImplSpec>& GetOutlet() {
    return outlet;
  }

  // TODO implicit conversion operator?

};

}

// for structured bindings
namespace std {

  template<typename ImplSpec>
  struct tuple_size<uit::Source<ImplSpec>>
    : std::integral_constant<size_t, 1>{};

  template<typename ImplSpec, size_t N>
  struct tuple_element<N, uit::Source<ImplSpec>> {

    using type = decltype(
      std::declval<uit::Source<ImplSpec>>().template get<N>()
    );
  };

}
