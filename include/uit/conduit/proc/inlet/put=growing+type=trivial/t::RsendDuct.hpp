#pragma once

#include "../../../../mpi/routine_functors.hpp"

#include "impl/TrivialBlockingSendDuct.hpp"

namespace uit {
namespace t {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class RsendDuct
: public uit::internal::TrivialBlockingSendDuct<
  uit::RsendFunctor,
  ImplSpec
> {

  // inherit parent's constructors
  // adapted from https://stackoverflow.com/a/434784
  using parent_t = uit::internal::TrivialBlockingSendDuct<
    uit::RsendFunctor,
    ImplSpec
  >;
  using parent_t::parent_t;

  /**
   * TODO.
   *
   * @return TODO.
   */
  static std::string GetName() { return "RsendDuct"; }

};

} // namespace t
} // namespace uit
