#pragma once

#include "../../../../../third-party/Empirical/source/base/vector.h"

#include "../../../datastructs/VectorMap.hpp"

#include "../../InterProcAddress.hpp"

#include "impl/InletMemoryAggregator.hpp"
#include "impl/OutletMemoryAggregator.hpp"
#include "impl/AggregatorSpec.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
// TODO use templating to share an implementation with PooledBackEnd
template<
  typename ImplSpec,
  template<typename> typename ProcDuct
>
class AggregatedBackEnd {

  using T = typename ImplSpec::T;

  using address_t = uit::InterProcAddress;

  using AggregatorSpec_t = uit::AggregatorSpec<ImplSpec, ProcDuct>;

public:

  using inlet_aggregator_t = uit::InletMemoryAggregator<AggregatorSpec_t>;
  using outlet_aggregator_t = uit::OutletMemoryAggregator<AggregatorSpec_t>;

private:

  // thread_id of caller -> proc_id of target -> inlet aggregator
  uit::VectorMap<
    uit::thread_id_t,
    uit::VectorMap<
      uit::proc_id_t,
      inlet_aggregator_t
    >
  > inlet_aggregators;

  // thread_id of caller -> proc_id of target -> outlet aggregator
  uit::VectorMap<
    uit::thread_id_t,
    uit::VectorMap<
      uit::proc_id_t,
      outlet_aggregator_t
    >
  > outlet_aggregators;

  bool AreAllInletAggregatorsInitialized() const {

    // check that all windows are in the same initialization state
    emp_assert( std::all_of(
      std::begin(inlet_aggregators), std::end(inlet_aggregators),
      [this](const auto& map_pair){
        const auto& [key, map] = map_pair;
        return std::all_of(
          std::begin(map), std::end(map),
          [this](const auto& aggregator_pair) {
            const auto& [key, aggregator] = aggregator_pair;
            const auto& rep = inlet_aggregators.begin()->second.begin()->second;
            return aggregator.IsInitialized() == rep.IsInitialized();
          }
        );
      }
    ) );


    return std::any_of(
      std::begin(inlet_aggregators), std::end(inlet_aggregators),
      [this](const auto& map_pair){
        const auto& [key, map] = map_pair;
        return std::any_of(
          std::begin(map), std::end(map),
          [this](const auto& aggregator_pair) {
            const auto& [key, aggregator] = aggregator_pair;
            return aggregator.IsInitialized();
          }
        );
      }
    );
  }

  bool AreAllOutletAggregatorsInitialized() const {

    // check that all windows are in the same initialization state
    emp_assert( std::all_of(
      std::begin(outlet_aggregators), std::end(outlet_aggregators),
      [this](const auto& map_pair){
        const auto& [key, map] = map_pair;
        return std::all_of(
          std::begin(map), std::end(map),
          [this](const auto& aggregator_pair) {
            const auto& [key, aggregator] = aggregator_pair;
            const auto& rep = outlet_aggregators.begin()->second.begin()->second;
            return aggregator.IsInitialized() == rep.IsInitialized()  ;
          }
        );
      }
    ) );

    return std::any_of(
      std::begin(outlet_aggregators), std::end(outlet_aggregators),
      [this](const auto& map_pair){
        const auto& [key, map] = map_pair;
        return std::any_of(
          std::begin(map), std::end(map),
          [this](const auto& aggregator_pair) {
            const auto& [key, aggregator] = aggregator_pair;
            return aggregator.IsInitialized();
          }
        );
      }
    );
  }

  bool IsInitialized() {
    emp_assert(
      AreAllInletAggregatorsInitialized() == AreAllOutletAggregatorsInitialized()
      || inlet_aggregators.empty()
      || outlet_aggregators.empty()
    );
    return AreAllInletAggregatorsInitialized()
    || AreAllOutletAggregatorsInitialized();
  }

  bool IsEmpty() {
    return outlet_aggregators.empty() && inlet_aggregators.empty();
  }

public:

  void RegisterInletSlot(const address_t& address) {
    emp_assert( !IsInitialized() );
    inlet_aggregators[
      address.GetInletThread()
    ][
      address.GetOutletProc()
    ].Register(address);
  }

  void RegisterOutletSlot(const address_t& address) {
    emp_assert( !IsInitialized() );
    outlet_aggregators[
      address.GetOutletThread()
    ][
      address.GetInletProc()
    ].Register(address);
  }

  void Initialize() {
    emp_assert( !IsInitialized() );

    for (auto& [__, proc_map] : inlet_aggregators) {
      for (auto& [__, aggregator] : proc_map) {
        aggregator.Initialize();
      }
    }
    for (auto& [__, proc_map] : outlet_aggregators) {
      for (auto& [__, aggregator] : proc_map) {
        aggregator.Initialize();
      }
    }

    emp_assert( IsInitialized() || IsEmpty() );
  }

  inlet_aggregator_t& GetInletAggregator(const address_t& address) {
    emp_assert( IsInitialized() );

    auto& aggregator = inlet_aggregators.at(
      address.GetInletThread()
    ).at(
      address.GetOutletProc()
    );

    emp_assert( aggregator.IsInitialized(), aggregator.GetSize() );

    return aggregator;
  }

  outlet_aggregator_t& GetOutletAggregator(const address_t& address) {
    emp_assert( IsInitialized() );

    auto& aggregator = outlet_aggregators.at(
      address.GetOutletThread()
    ).at(
      address.GetInletProc()
    );

    emp_assert( aggregator.IsInitialized() );

    return aggregator;
  }

  constexpr static bool CanStep() { return outlet_aggregator_t::CanStep(); }

};

} // namespace uit
