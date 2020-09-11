#pragma once

#include <type_traits>

#include "../inlet/put=growing+type=cereal/CerealRsendDuct.hpp"
#include "../outlet/get=stepping+type=cereal/CerealIprobeDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IcrOci {

  using InletImpl = uit::CerealRsendDuct<ImplSpec>;
  using OutletImpl = uit::CerealIprobeDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace uit
