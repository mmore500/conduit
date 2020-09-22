#pragma once

#include <type_traits>

#include "../impl/inlet/templated/BufferedInletDuct.hpp"
#include "../impl/outlet/templated/BufferedOutletDuct.hpp"

#include "../put=dropping+get=stepping+type=span/inlet=RingIsend+outlet=Iprobe_s::IriOiDuct.hpp"

namespace uit {
namespace t {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class BufferedIriOiDuct {

  template<typename Spec>
  using BackingDuct = uit::s::IriOiDuct<Spec>;

public:

  using InletImpl = uit::BufferedInletDuct<BackingDuct, ImplSpec>;
  using OutletImpl = uit::BufferedOutletDuct<BackingDuct, ImplSpec>;

  static_assert(std::is_same<
    typename InletImpl::BackEndImpl,
    typename OutletImpl::BackEndImpl
  >::value);

  using BackEndImpl = typename InletImpl::BackEndImpl;

};

} // namespace t
} // namespace uit
