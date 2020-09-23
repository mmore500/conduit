#pragma once

#include "../../../../../../uitsl/mpi/routine_functors.hpp"

#include "impl/SpanRingImmediateSendDuct.hpp"

namespace uit {
namespace s {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class RingIrsendDuct
: public uit::internal::SpanRingImmediateSendDuct<
  uitsl::IrsendFunctor,
  ImplSpec
> {

  // inherit parent's constructors
  // adapted from https://stackoverflow.com/a/434784
  using parent_t = uit::internal::SpanRingImmediateSendDuct<
    uitsl::IrsendFunctor,
    ImplSpec
  >;
  using parent_t::parent_t;


  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "RingIrsendDuct"; }

};

} // namespace s
} // namespace uit
