#pragma once

#include <type_traits>

#include "../inlet/accumulating+type=trivial/t::BufferedIsendDuct.hpp"
#include "../outlet/accumulating+type=trivial/t::IrecvDuct.hpp"

namespace uit {
namespace t {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct BufferedIiOiDuct {

  using InletImpl = uit::t::BufferedIsendDuct<ImplSpec>;
  using OutletImpl = uit::t::IrecvDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace t
} // namespace uit
