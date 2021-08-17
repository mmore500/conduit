#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_INSTRUMENTATIONAGGREGATINGSPOUTWRAPPER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_INSTRUMENTATIONAGGREGATINGSPOUTWRAPPER_HPP_INCLUDE

#include "inlet/InstrumentationAggregatingInletWrapper.hpp"
#include "outlet/InstrumentationAggregatingOutletWrapper.hpp"

namespace uit {

template<typename T_>
class InstrumentationAggregatingSpoutWrapper {

public:
  using T = T_;

  template<typename Inlet>
  using inlet_wrapper_t
    = uit::internal::InstrumentationAggregatingInletWrapper<Inlet>;

  template<typename Outlet>
  using outlet_wrapper_t
    = uit::internal::InstrumentationAggregatingOutletWrapper<Outlet>;

};


} // namespace uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_INSTRUMENTATIONAGGREGATINGSPOUTWRAPPER_HPP_INCLUDE
