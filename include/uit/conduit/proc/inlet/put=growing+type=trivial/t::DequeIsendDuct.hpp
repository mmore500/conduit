#pragma once

#include "../../../../mpi/routine_functors.hpp"

#include "impl/TrivialDequeImmediateSendDuct.hpp"

namespace uit {
namespace t {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class DequeIsendDuct
: public uit::internal::TrivialDequeImmediateSendDuct<
  uit::IsendFunctor,
  ImplSpec
> {

  // inherit parent's constructors
  // adapted from https://stackoverflow.com/a/434784
  using parent_t = uit::internal::TrivialDequeImmediateSendDuct<
    uit::IsendFunctor,
    ImplSpec
  >;
  using parent_t::parent_t;

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "DequeIsendDuct"; }

};

} // namespace t
} // namespace uit
