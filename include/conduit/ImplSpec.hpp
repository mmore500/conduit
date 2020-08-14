#pragma once

#include "intra/PendingDuct.hpp"
#include "thread/HeadTailDuct.hpp"
#include "proc/ImsgDuct.hpp"
#include "config.hpp"

namespace uit {

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

template<
  typename T_,
  size_t N_=DEFAULT_BUFFER,
  typename ImplSelector=uit::ImplSelector<>
>
struct ImplSpec {

  using THIS_T = ImplSpec<T_, N_, ImplSelector>;

  using T = T_;

  constexpr inline static size_t N{ N_ };

  using IntraDuct = typename ImplSelector::template IntraDuct<THIS_T>;

  using ThreadDuct = typename ImplSelector::template ThreadDuct<THIS_T>;

  using ProcInletDuct = typename ImplSelector::template
    ProcDuct<THIS_T>::InletImpl;

  using ProcOutletDuct = typename ImplSelector::template
    ProcDuct<THIS_T>::OutletImpl;

  // TODO add static ToString

};


}
