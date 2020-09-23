#pragma once

#include <type_traits>

#include "../inlet/accumulating+type=fundamental/f::RaccumulateDuct.hpp"
#include "../outlet/accumulating+type=fundamental/f::WithdrawingWindowDuct.hpp"

namespace uit {
namespace f {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IrOwwDuct {

  using InletImpl = uit::f::RaccumulateDuct<ImplSpec>;
  using OutletImpl = uit::f::WithdrawingWindowDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace f
} // namespace uit
