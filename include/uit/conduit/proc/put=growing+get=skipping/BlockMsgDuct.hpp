#pragma once

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

};

} // namespace uit
