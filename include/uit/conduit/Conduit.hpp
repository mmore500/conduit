#pragma once

#include <memory>
#include <stddef.h>
#include <utility>

#include "../duct/Duct.hpp"

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
 *   `include/config/ImplSpec.hpp`.
 */
template<typename ImplSpec>
class Conduit {

  /// Typedef for `Duct` implementation.
  using duct_t = uit::internal::Duct<ImplSpec>;

  /// Handle to `Duct` shared by `inlet` and `outlet`.
  std::shared_ptr<duct_t> duct;

  /// `Conduit`'s `Inlet`.
  uit::Inlet<ImplSpec> inlet;
  /// `Conduit`'s `Outlet`.
  uit::Outlet<ImplSpec> outlet;

public:

  /**
   * Copy constructor.
   */
  Conduit(Conduit& other) = default;

  /**
   * Copy constructor.
   */
  Conduit(const Conduit& other) = default;

  /**
   * Move constructor.
   */
  Conduit(Conduit&& other) = default;

  /**
   * Forwarding constructor.
   *
   * Use `std::in_place_type_t<ImplType>{}` followed by constructor arguments to
   * initialize the `Duct` with `ImplType` active.
   */
  template <typename... Args>
  Conduit(Args&&... args) : duct(
    std::make_shared<internal::Duct<ImplSpec>>( std::forward<Args>(args)... )
  ), inlet(duct)
  , outlet(duct)
  { ; }

  /**
   * Adaptor for structured bindings as interface to access `Conduit`'s `Inlet`
   * or `Outlet`.
   *
   * @tparam N Index of element to access, 0 for `Inlet` or 1 for `Outlet`.
   * @return `Conduit`'s `Inlet` or `Outlet`.
   */
  template <size_t N>
  decltype(auto) get() const {
    if constexpr (N == 0) return inlet;
    else if constexpr (N == 1) return outlet;
  }

  /**
   * Accessor for `Conduit`'s `Inlet`.
   *
   * @return `Conduit`'s `Inlet`.
   */
  uit::Inlet<ImplSpec> & GetInlet() { return inlet; }

  /**
   * Accessor for `Conduit`'s `Outlet`.
   *
   * @return `Conduit`'s `Inlet`.
   */
  uit::Outlet<ImplSpec> & GetOutlet() { return outlet; }

};

} // namespace uit

namespace std {

  /// Adaptor for structured bindings.
  template<typename ImplSpec>
  struct tuple_size<uit::Conduit<ImplSpec>>
    : std::integral_constant<size_t, 2>{};

  /// Adaptor for structured bindings.
  template<typename ImplSpec, size_t N>
  struct tuple_element<N, uit::Conduit<ImplSpec>> {
    using type = decltype(
      std::declval<uit::Conduit<ImplSpec>>().template get<N>()
    );
  };

} // namespace std
