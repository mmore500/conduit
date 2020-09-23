#pragma once

#include "../../../../../../third-party/Empirical/source/base/vector.h"

#include "../../../mock/ThrowDuct.hpp"

namespace uit {

template<
  typename ImplSpec,
  template<typename> typename ProcDuct
>
class PoolSpec {

  using THIS_T = PoolSpec<ImplSpec, ProcDuct>;

public:

  using T = emp::vector<typename ImplSpec::T>;
  constexpr inline static size_t N{ ImplSpec::N };
  constexpr inline static size_t B{ ImplSpec::B };

  using IntraDuct = uit::ThrowDuct<THIS_T>;
  using ThreadDuct = uit::ThrowDuct<THIS_T>;

  using ProcInletDuct = typename ProcDuct<THIS_T>::InletImpl;
  using ProcOutletDuct = typename ProcDuct<THIS_T>::OutletImpl;
  using ProcBackEnd = typename ProcDuct<THIS_T>::BackEndImpl;

};

} // namespace uit
