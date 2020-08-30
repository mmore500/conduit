#pragma once

#include <atomic>

#include "../impl/PendingDuct.hpp"
#include "../../parallel/AlignedImplicit.hpp"
#include "../../parallel/AlignedInherit.hpp"

namespace uit {

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

} // namespace uit
