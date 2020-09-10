#pragma once

#include <type_traits>

#include "../inlet/templated/PooledInletDuct.hpp"
#include "../outlet/templated/PooledOutletDuct.hpp"

#include "inlet=VectorRingIsend+outlet=VectorBlockIrecv_IvriOvbiDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class PooledIvriOvbiDuct {

  template<typename Spec>
  using BackingDuct = uit::IvriOvbiDuct<Spec>;

public:

  using InletImpl = uit::PooledInletDuct<BackingDuct, ImplSpec>;
  using OutletImpl = uit::PooledOutletDuct<BackingDuct, ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace uit
