#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_TEMPLATED_IMPL_BUFFERSPEC_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_TEMPLATED_IMPL_BUFFERSPEC_HPP_INCLUDE

#include <vector>

#include "../../../../../mock/ThrowDuct.hpp"

namespace uit {
namespace internal {

template<
  typename ImplSpec,
  template<typename> typename ProcDuct
>
class BufferSpecKernel {

  using THIS_T = BufferSpecKernel<ImplSpec, ProcDuct>;

public:

  using T = std::vector<typename ImplSpec::T>;
  constexpr inline static size_t N{ ImplSpec::N };
  constexpr inline static size_t B{ ImplSpec::B };

  using IntraDuct = uit::ThrowDuct<THIS_T>;
  using ThreadDuct = uit::ThrowDuct<THIS_T>;

};

template<
  typename ImplSpec,
  template<typename> typename ProcDuct
>
class BufferSpec : public BufferSpecKernel<ImplSpec, ProcDuct> {

  using parent_t = BufferSpecKernel<ImplSpec, ProcDuct>;

  using ProcInletDuct = typename ProcDuct<parent_t>::InletImpl;
  using ProcOutletDuct = typename ProcDuct<parent_t>::OutletImpl;
  using ProcBackEnd = typename ProcDuct<parent_t>::BackEndImpl;

};

} // namespace internal
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_TEMPLATED_IMPL_BUFFERSPEC_HPP_INCLUDE
