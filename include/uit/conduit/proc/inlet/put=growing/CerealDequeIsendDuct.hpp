#pragma once

#include "../../../../distributed/IsendFunctor.hpp"

#include "impl/CerealDequeImmediateSendDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class CerealDequeIsendDuct
: public uit::internal::CerealDequeImmediateSendDuct<
  uit::IsendFunctor,
  ImplSpec
> {

  // inherit parent's constructors
  // adapted from https://stackoverflow.com/a/434784
  using parent_t = uit::internal::CerealDequeImmediateSendDuct<
    uit::IsendFunctor,
    ImplSpec
  >;
  using parent_t::parent_t;

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "CerealDequeIsendDuct"; }

};

} // namespace uit
