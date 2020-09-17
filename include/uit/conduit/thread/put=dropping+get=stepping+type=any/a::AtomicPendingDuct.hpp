#pragma once

#include <atomic>

#include "../../../../uitsl/parallel/AlignedImplicit.hpp"
#include "../../../../uitsl/parallel/AlignedInherit.hpp"

#include "../../intra/put=dropping+get=stepping+type=any/impl/PendingDuct.hpp"


namespace uit {
namespace a {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class AtomicPendingDuct
: public uit::internal::PendingDuct<
  uit::AlignedInherit<std::atomic<size_t>>,
  uit::AlignedImplicit<typename ImplSpec::T>,
  ImplSpec
>
{

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "AtomicPendingDuct"; }

};

} // namespace a
} // namespace uit
