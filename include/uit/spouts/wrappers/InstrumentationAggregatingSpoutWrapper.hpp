#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_INSTRUMENTATIONAGGREGATINGSPOUTWRAPPER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_INSTRUMENTATIONAGGREGATINGSPOUTWRAPPER_HPP_INCLUDE

#include <string>
#include <tuple>

#include "inlet/InstrumentationAggregatingInletWrapper.hpp"
#include "outlet/InstrumentationAggregatingOutletWrapper.hpp"

#include "impl/RoundTripCountPacket.hpp"

namespace uit {

template<typename T_>
class InstrumentationAggregatingSpoutWrapper {

public:
  using T = uit::impl::RoundTripCountPacket<T_>;

  template<typename Inlet>
  using inlet_wrapper_t
    = uit::internal::InstrumentationAggregatingInletWrapper<Inlet>;

  template<typename Outlet>
  using outlet_wrapper_t
    = uit::internal::InstrumentationAggregatingOutletWrapper<Outlet>;


  template<typename Inlet>
  decltype(auto) MakeInletContainerDataFile(const std::string& filename) {
    return inlet_wrapper_t<Inlet>::MakeContainerDataFile(filename);
  }

  template<typename Inlet>
  decltype(auto) MakeInletSummaryDataFile(const std::string& filename) {
    return inlet_wrapper_t<Inlet>::MakeSummaryDataFile(filename);
  }

  template<typename Outlet>
  decltype(auto) MakeOutletContainerDataFile(const std::string& filename) {
    return outlet_wrapper_t<Outlet>::MakeContainerDataFile(filename);
  }

  template<typename Outlet>
  decltype(auto) MakeOutletSummaryDataFile(const std::string& filename) {
    return outlet_wrapper_t<Outlet>::MakeSummaryDataFile(filename);
  }

};


} // namespace uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_INSTRUMENTATIONAGGREGATINGSPOUTWRAPPER_HPP_INCLUDE
