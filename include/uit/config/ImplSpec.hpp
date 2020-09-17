#pragma once

#include "defaults.hpp"

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
  template<typename> typename IntraDuct_ = uit::DefaultIntraDuct,
  template<typename> typename ThreadDuct_ = uit::DefaultThreadDuct,
  template<typename> typename ProcDuct_ = uit::DefaultProcDuct
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
  size_t N_,
  typename ImplSelect,
  size_t B_
>
class ImplSpecKernel {

  /// TODO.
  using THIS_T = ImplSpecKernel<T_, N_, ImplSelect, B_>;

public:

  /// TODO.
  using T = T_;

  /// TODO.
  constexpr inline static size_t N{ N_ };

  /// TODO.
  /// TODO.
  constexpr inline static size_t B{ B_ };

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

  /// TODO.
  using ProcBackEnd = typename ImplSelect::template
    ProcDuct<THIS_T>::BackEndImpl;

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
 */
template<
  typename T,
  size_t N=uit::DEFAULT_BUFFER,
  typename ImplSelect=uit::ImplSelect<>,
  size_t B=std::numeric_limits<size_t>::max()
>
class ImplSpec : public internal::ImplSpecKernel<T, N, ImplSelect, B> { };

} // namespace uit
