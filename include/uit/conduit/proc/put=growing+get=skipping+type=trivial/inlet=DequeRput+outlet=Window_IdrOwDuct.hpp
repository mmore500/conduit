#pragma once

#include <type_traits>

#include "../inlet/put=growing+type=trivial/DequeRputDuct.hpp"
#include "../outlet/get=skipping+type=trivial/WindowDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IdrOwDuct {

  using InletImpl = uit::DequeRputDuct<ImplSpec>;
  using OutletImpl = uit::WindowDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace uit
