#pragma once

#include <type_traits>

#include "../inlet/put=dropping/VectorRingIsendDuct.hpp"
#include "../outlet/get=stepping/VectorIprobeDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IvriOviDuct {

  using InletImpl = uit::VectorRingIsendDuct<ImplSpec>;
  using OutletImpl = uit::VectorIprobeDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace uit
