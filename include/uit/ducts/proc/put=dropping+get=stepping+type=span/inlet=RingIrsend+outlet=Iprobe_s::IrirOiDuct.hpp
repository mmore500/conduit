#pragma once

#include <type_traits>

#include "../impl/inlet/put=dropping+type=span/s::RingIrsendDuct.hpp"
#include "../impl/outlet/get=stepping+type=span/s::IprobeDuct.hpp"

namespace uit {
namespace s {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IrirOriDuct {

  using InletImpl = uit::s::RingIrsendDuct<ImplSpec>;
  using OutletImpl = uit::s::IprobeDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace s
} // namespace uit
