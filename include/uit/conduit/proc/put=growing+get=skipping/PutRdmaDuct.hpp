#pragma once

#include <type_traits>

#include "../inlet/put=growing/PutDuct.hpp"
#include "../outlet/get=skipping/WindowDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct PutRdmaDuct {

  using InletImpl = uit::PutDuct<ImplSpec>;
  using OutletImpl = uit::WindowDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace uit
