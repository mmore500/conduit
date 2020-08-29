#pragma once

#include "../../../distributed/SendFunctor.hpp"

#include "../../impl/BlockingSendDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class SendDuct
: public uit::internal::BlockingSendDuct<
  uit::SendFunctor,
  ImplSpec
> {

  // inherit parent's constructors
  // adapted from https://stackoverflow.com/a/434784
  using parent_t = uit::internal::BlockingSendDuct<
    uit::SendFunctor,
    ImplSpec
  >;
  using parent_t::parent_t;

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "SendDuct"; }

};

} // namespace uit
