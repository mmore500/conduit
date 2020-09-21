#pragma once

#include <type_traits>

#include "../inlet/accumulating+type=fundamental/f::IsendDuct.hpp"
#include "../outlet/accumulating+type=fundamental/f::IrecvDuct.hpp"

namespace uit {
namespace f {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
struct IiOiDuct {

  using InletImpl = uit::f::IsendDuct<ImplSpec>;
  using OutletImpl = uit::f::IrecvDuct<ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace f
} // namespace uit
