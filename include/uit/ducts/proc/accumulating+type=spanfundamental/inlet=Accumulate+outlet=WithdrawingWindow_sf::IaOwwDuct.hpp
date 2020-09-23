#pragma once

#include <type_traits>

#include "../impl/inlet/accumulating+type=spanfundamental/sf::AccumulateDuct.hpp"
#include "../impl/outlet/accumulating+type=spanfundamental/sf::WithdrawingWindowDuct.hpp"

namespace uit {
namespace sf {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IaOwwDuct {

  using InletImpl = uit::sf::AccumulateDuct<ImplSpec>;
  using OutletImpl = uit::sf::WithdrawingWindowDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace sf
} // namespace uit
