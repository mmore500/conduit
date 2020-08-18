#pragma once

#include <memory>
#include <stddef.h>
#include <utility>

#include "Duct.hpp"
#include "Inlet.hpp"
#include "Outlet.hpp"

namespace uit {

/**
 * Creates a coupled `Inlet` and `Outlet`.
 *
 * Useful for intra-process transmission (i.e., intra-thread or inter-thread
 * transmission) where conceptually-coupled `Inlet`'s and `Outlet`'s exist
 * within the same memory space.
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework. See
 *   `include/conduit/ImplSpec.hpp`.
 */
template<typename ImplSpec>
class Conduit {

  /// TODO.
  using duct_t = uit::Duct<ImplSpec>;

  /// TODO.
  std::shared_ptr<duct_t> duct{ std::make_shared<duct_t>() };

  /// TODO.
  uit::Inlet<ImplSpec> inlet{ duct };
  /// TODO.
  uit::Outlet<ImplSpec> outlet{ duct };

public:

  // for structured bindings
  /**
   * TODO.
   *
   * @tparam N TODO.
   * @return TODO.
   */
  template <size_t N>
  decltype(auto) get() const {
      // parens needed to get reference?
      if constexpr (N == 0) return (inlet);
      else if constexpr (N == 1) return (outlet);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  uit::Inlet<ImplSpec> & GetInlet() { return inlet; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  uit::Outlet<ImplSpec> & GetOutlet() { return outlet; }

  // TODO implicit conversion operators?


};

} // namespace uit

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

} // namespace std
