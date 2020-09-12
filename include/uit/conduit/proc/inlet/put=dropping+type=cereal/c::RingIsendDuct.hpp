#pragma once

#include "../../../../mpi/routine_functors.hpp"

#include "impl/CerealRingImmediateSendDuct.hpp"

namespace uit {
namespace c {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class RingIsendDuct
: public uit::internal::CerealRingImmediateSendDuct<
  uit::IsendFunctor,
  ImplSpec
> {

  // inherit parent's constructors
  // adapted from https://stackoverflow.com/a/434784
  using parent_t = uit::internal::CerealRingImmediateSendDuct<
    uit::IsendFunctor,
    ImplSpec
  >;
  using parent_t::parent_t;


  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "RingIsendDuct"; }

};

} // namespace c
} // namespace uit
