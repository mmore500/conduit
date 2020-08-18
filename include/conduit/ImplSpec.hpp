#pragma once

#include "config.hpp"

#include "intra/PendingDuct.hpp"
#include "proc/ImsgDuct.hpp"
#include "thread/HeadTailDuct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam IntraDuct_ TODO.
 * @tparam ThreadDuct_ TODO.
 * @tparam ProcDuct_ TODO.
 */
template<
  template<typename> typename IntraDuct_ = uit::PendingDuct,
  template<typename> typename ThreadDuct_ = uit::HeadTailDuct,
  template<typename> typename ProcDuct_ = uit::ImsgDuct
>
struct ImplSelector {

  template<typename ImplSpec>
  using IntraDuct = IntraDuct_<ImplSpec>;

  template<typename ImplSpec>
  using ThreadDuct = ThreadDuct_<ImplSpec>;

  template<typename ImplSpec>
  using ProcDuct = ProcDuct_<ImplSpec>;

};

/**
 * TODO
 *
 * @tparam T_ TODO.
 * @tparam N_ TODO.
 * @tparam ImplSelector TODO.
 */
template<
  typename T_,
  size_t N_=DEFAULT_BUFFER,
  typename ImplSelector=uit::ImplSelector<>
>
class ImplSpec {

  /// TODO.
  using THIS_T = ImplSpec<T_, N_, ImplSelector>;

public:

  /// TODO.
  using T = T_;

  /// TODO.
  constexpr inline static size_t N{ N_ };

  /// TODO.
  using IntraDuct = typename ImplSelector::template IntraDuct<THIS_T>;

  /// TODO.
  using ThreadDuct = typename ImplSelector::template ThreadDuct<THIS_T>;

  /// TODO.
  using ProcInletDuct = typename ImplSelector::template
    ProcDuct<THIS_T>::InletImpl;

  /// TODO.
  using ProcOutletDuct = typename ImplSelector::template
    ProcDuct<THIS_T>::OutletImpl;

  // TODO add static ToString

};


} // namespace uit
