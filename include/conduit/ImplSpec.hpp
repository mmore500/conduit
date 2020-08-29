#pragma once

#include "config.hpp"

#include "intra/SerialPendingDuct.hpp"
#include "proc/ImsgDuct.hpp"
#include "thread/HeadTailDuct.hpp"

namespace uit {

/**
 * Specifies the `Duct` implementations to be used for intra-thread, inter-
 * thread, and inter-process transmission.
 *
 * @tparam IntraDuct_ Implementation to use for intra-thread transmission.
 * @tparam ThreadDuct_ Implementation to use for inter-thread transmission.
 * @tparam ProcDuct_ Implementation to use for inter-process transmission
 */
template<
  template<typename> typename IntraDuct_ = uit::SerialPendingDuct,
  template<typename> typename ThreadDuct_ = uit::HeadTailDuct,
  template<typename> typename ProcDuct_ = uit::ImsgDuct
>
struct ImplSelect {

  template<typename ImplSpec>
  using IntraDuct = IntraDuct_<ImplSpec>;

  template<typename ImplSpec>
  using ThreadDuct = ThreadDuct_<ImplSpec>;

  template<typename ImplSpec>
  using ProcDuct = ProcDuct_<ImplSpec>;

};

namespace internal {

template<
  typename T_,
  size_t N_=DEFAULT_BUFFER,
  typename ImplSelect=uit::ImplSelect<>
>
class ImplSpecKernel {

  /// TODO.
  using THIS_T = ImplSpecKernel<T_, N_, ImplSelect>;

public:

  /// TODO.
  using T = T_;

  /// TODO.
  constexpr inline static size_t N{ N_ };

  /// TODO.
  using IntraDuct = typename ImplSelect::template IntraDuct<THIS_T>;

  /// TODO.
  using ThreadDuct = typename ImplSelect::template ThreadDuct<THIS_T>;

  /// TODO.
  using ProcInletDuct = typename ImplSelect::template
    ProcDuct<THIS_T>::InletImpl;

  /// TODO.
  using ProcOutletDuct = typename ImplSelect::template
    ProcDuct<THIS_T>::OutletImpl;


  // TODO add static ToString

};

} // namespace internal

/**
 * Specifies implementation details for the conduit framework.
 *
 * @tparam T Type to transmit.
 * @tparam N Buffer size.
 * @tparam ImplSelect Class with static typedef members specifying which
 * implementations to use for intra-thread, inter-thread, and inter-process
 * transmission.
 *
 * @note The type `T_` should be *TriviallyCopyable*.
 */
template<
  typename T,
  size_t N=DEFAULT_BUFFER,
  typename ImplSelect=uit::ImplSelect<>
>
class ImplSpec : public internal::ImplSpecKernel<T, N, ImplSelect> {

  using parent_t = internal::ImplSpecKernel<T, N, ImplSelect>;

  static_assert(std::is_same<
    typename parent_t::ProcInletDuct::BackEndImpl,
    typename parent_t::ProcOutletDuct::BackEndImpl
  >::value);

public:

  // TODO.
  using ProcBackEnd = typename parent_t::ProcInletDuct::BackEndImpl;

};

} // namespace uit
