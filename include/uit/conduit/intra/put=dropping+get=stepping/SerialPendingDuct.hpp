#pragma once

#include "impl/PendingDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class SerialPendingDuct
: public uit::internal::PendingDuct<
  size_t,
  typename ImplSpec::T,
  ImplSpec
> {

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "SerialPendingDuct"; }

};

} // namespace uit
