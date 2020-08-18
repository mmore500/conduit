#pragma once

#include <memory>
#include <stddef.h>
#include <utility>

#include "Duct.hpp"
#include "Inlet.hpp"

namespace uit {

/**
 * Creates an uncoupled `Inlet`.
 *
 * Potentially useful for inter-process transmission, where a conceptually-
 * coupled `Inlet` and `Outlet` must be constructed within separate memory
 * spaces.
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework. See
 *   `include/conduit/ImplSpec.hpp`.
 */
template<typename ImplSpec>
class Sink {

  uit::Inlet<ImplSpec> inlet{
    std::make_shared<uit::Duct<ImplSpec>>()
  };

public:

  // for structured bindings
  /**
   * TODO.
   *
   * @return TODO.
   */
  template <size_t N>
  decltype(auto) get() const {
      // parens needed to get reference?
      if constexpr (N == 0) return (inlet);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  uit::Inlet<ImplSpec>& GetInlet() {
    return inlet;
  }

  // TODO implicit conversion operator?

};

} // namespace uit

// for structured bindings
namespace std {

  template<typename ImplSpec>
  struct tuple_size<uit::Sink<ImplSpec>>
    : std::integral_constant<size_t, 1>{};

  template<typename ImplSpec, size_t N>
  struct tuple_element<N, uit::Sink<ImplSpec>> {

    using type = decltype(
      std::declval<uit::Sink<ImplSpec>>().template get<N>()
    );
  };

} // namespace std
