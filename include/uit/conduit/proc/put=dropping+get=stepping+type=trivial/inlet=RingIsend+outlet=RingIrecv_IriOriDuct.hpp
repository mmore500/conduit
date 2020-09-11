#pragma once

#include <type_traits>

#include "../inlet/put=dropping+type=trivial/RingIsendDuct.hpp"
#include "../outlet/get=stepping+type=trivial/RingIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IriOriDuct {

  using InletImpl = uit::RingIsendDuct<ImplSpec>;
  using OutletImpl = uit::RingIrecvDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace uit
