#pragma once

#include <type_traits>

#include "../inlet/accumulating/RaccumulateDuct.hpp"
#include "../outlet/accumulating/WithdrawingWindowDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IrOwwDuct {

  using InletImpl = uit::RaccumulateDuct<ImplSpec>;
  using OutletImpl = uit::WithdrawingWindowDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace uit
