#pragma once

#include "../../../distributed/IsendFunctor.hpp"

#include "../../impl/ImmediateSendDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class IsendDuct
: public uit::internal::ImmediateSendDuct<
  uit::IsendFunctor,
  ImplSpec
> {

  // inherit parent's constructors
  // adapted from https://stackoverflow.com/a/434784
  using parent_t = uit::internal::ImmediateSendDuct<
    uit::IsendFunctor,
    ImplSpec
  >;
  using parent_t::parent_t;


  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "IsendDuct"; }

};

} // namespace uit
