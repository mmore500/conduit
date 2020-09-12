#pragma once

#include <type_traits>

#include "../inlet/put=dropping+type=trivial/t::RingIrsendDuct.hpp"
#include "../outlet/get=skipping+type=trivial/t::BlockIrecvDuct.hpp"

namespace uit {
namespace t {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IrirObiDuct {

  using InletImpl = uit::t::RingIrsendDuct<ImplSpec>;
  using OutletImpl = uit::t::BlockIrecvDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace t
} // namespace uit
