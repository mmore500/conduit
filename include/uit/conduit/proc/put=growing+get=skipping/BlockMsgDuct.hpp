#pragma once

#include <type_traits>

#include "../inlet/put=growing/SendDuct.hpp"
#include "../outlet/get=skipping/BlockIrecvDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct BlockMsgDuct {

  using InletImpl = uit::SendDuct<ImplSpec>;
  using OutletImpl = uit::BlockIrecvDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace uit
