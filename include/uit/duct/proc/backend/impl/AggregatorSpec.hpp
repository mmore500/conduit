#pragma once

#include <map>

#include "../../../../../../third-party/cereal/include/cereal/types/map.hpp"

#include "../../../mock/ThrowDuct.hpp"

namespace uit {

template<
  typename ImplSpec,
  template<typename> typename ProcDuct
>
class AggregatorSpec {

  using THIS_T = AggregatorSpec<ImplSpec, ProcDuct>;

public:

  using T = std::multimap< int, typename ImplSpec::T >;
  constexpr inline static size_t N{ ImplSpec::N };
  constexpr inline static size_t B{ ImplSpec::B };

  using IntraDuct = uit::ThrowDuct<THIS_T>;
  using ThreadDuct = uit::ThrowDuct<THIS_T>;

  using ProcInletDuct = typename ProcDuct<THIS_T>::InletImpl;
  using ProcOutletDuct = typename ProcDuct<THIS_T>::OutletImpl;
  using ProcBackEnd = typename ProcDuct<THIS_T>::BackEndImpl;

};

} // namespace uit
