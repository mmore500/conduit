#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_INLET_INSTRUMENTATIONAGGREGATINGINLETWRAPPER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_INLET_INSTRUMENTATIONAGGREGATINGINLETWRAPPER_HPP_INCLUDE

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <memory>
#include <optional>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "../../../../uit_emp/data/DataFile.hpp"
#include "../../../../uit_emp/tools/string_utils.hpp"

#include "../../../../uitsl/algorithm/accumulate_if.hpp"
#include "../../../../uitsl/containers/safe/unordered_set.hpp"
#include "../../../../uitsl/countdown/coarse_runtime.hpp"
#include "../../../../uitsl/debug/benchmark_utils.hpp"
#include "../../../../uitsl/mpi/comm_utils.hpp"
#include "../../../../uitsl/parallel/thread_utils.hpp"

#include "../impl/RoundTripCounterAddr.hpp"
#include "../impl/RoundTripCountPacket.hpp"
#include "../impl/round_trip_touch_counter.hpp"

namespace uit {
namespace internal {

template<typename Inlet>
class InstrumentationAggregatingInletWrapper {

  using ImplSpec = typename Inlet::ImplSpec;
  using this_t = InstrumentationAggregatingInletWrapper<Inlet>;

  using inlet_t = Inlet;
  inlet_t inlet;

  using value_type = typename ImplSpec::value_type;

  inline static uitsl::safe::unordered_set<const this_t*> registry;

  template<typename Filter>
  struct RegistryAggregator {

    static size_t GetNumPutsAttempted() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* inlet) {
          return accum + inlet->GetNumPutsAttempted();
        },
        Filter{}
      );
    }

    static size_t GetNumTryPutsAttempted() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* inlet) {
          return accum + inlet->GetNumTryPutsAttempted();
        },
        Filter{}
      );
    }

    static size_t GetNumBlockingPuts() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* inlet) {
          return accum + inlet->GetNumBlockingPuts();
        },
        Filter{}
      );
    }

    static size_t GetNumTryPutsThatSucceeded() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* inlet) {
          return accum + inlet->GetNumTryPutsThatSucceeded();
        },
        Filter{}
      );
    }

    static size_t GetNumPutsThatSucceededEventually() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* inlet) {
          return accum + inlet->GetNumTryPutsAttempted();
        },
        Filter{}
      );
    }

  static size_t GetNumBlockingPutsThatSucceededImmediately() {
    std::shared_lock lock{ registry.GetMutex() };
    return uitsl::accumulate_if(
      std::begin(registry), std::end(registry), size_t{},
      [](size_t accum, const this_t* inlet) {
        return accum + inlet->GetNumBlockingPutsThatSucceededImmediately();
      },
      Filter{}
    );
  }

    static size_t GetNumPutsThatSucceededImmediately() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* inlet) {
          return accum + inlet->GetNumPutsThatSucceededImmediately();
        },
        Filter{}
      );
    }

    static size_t GetNumPutsThatBlocked() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* inlet) {
          return accum + inlet->GetNumPutsThatBlocked();
        },
        Filter{}
      );
    }

    static size_t GetNumDroppedPuts() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* inlet) {
          return accum + inlet->GetNumDroppedPuts();
        },
        Filter{}
      );
    }

    static double GetFractionTryPutsDropped() {
      struct Adder {
        size_t num_dropped_puts{};
        size_t num_try_puts_attempted{};
        double GetRatio() const {
          return num_dropped_puts / static_cast<double>(
            num_try_puts_attempted
          );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_dropped_puts += inlet->GetNumDroppedPuts();
          accum.num_try_puts_attempted += inlet->GetNumTryPutsAttempted();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static double GetFractionTryPutsThatSucceeded() {
      struct Adder {
        size_t num_try_puts_that_succeeded{};
        size_t num_try_puts_attempted{};
        double GetRatio() const {
          return num_try_puts_that_succeeded / static_cast<double>(
            num_try_puts_attempted
          );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_try_puts_that_succeeded
            += inlet->GetNumTryPutsThatSucceeded();
          accum.num_try_puts_attempted += inlet->GetNumTryPutsAttempted();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static double GetFractionBlockingPutsThatBlocked() {
      struct Adder {
        size_t num_puts_that_blocked{};
        size_t num_blocking_puts{};
        double GetRatio() const {
          return num_puts_that_blocked / static_cast<double>(
            num_blocking_puts
          );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_puts_that_blocked += inlet->GetNumPutsThatBlocked();
          accum.num_blocking_puts += inlet->GetNumBlockingPuts();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static double GetFractionPutsThatSucceededEventually() {
      struct Adder {
        size_t num_puts_that_succeeded_eventually{};
        size_t num_puts_attempted{};
        double GetRatio() const {
          return num_puts_that_succeeded_eventually / static_cast<double>(
            num_puts_attempted
          );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_puts_that_succeeded_eventually
            += inlet->GetNumPutsThatSucceededEventually();
          accum.num_puts_attempted += inlet->GetNumPutsAttempted();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static double GetFractionPutsThatSucceededImmediately() {
      struct Adder {
        size_t num_puts_that_succeeded_immediately{};
        size_t num_puts_attempted{};
        double GetRatio() const {
          return num_puts_that_succeeded_immediately / static_cast<double>(
            num_puts_attempted
          );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_puts_that_succeeded_immediately
            += inlet->GetNumPutsThatSucceededImmediately();
          accum.num_puts_attempted += inlet->GetNumPutsAttempted();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static double GetRoundTripTouchesPerAttemptedPut() {
      struct Adder {
        size_t num_round_trip_touches{};
        size_t num_puts_attempted{};
        double GetRatio() const {
          return num_round_trip_touches / static_cast<double>(
            num_puts_attempted
          );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_round_trip_touches += inlet->GetCurRoundTripTouchCount();
          accum.num_puts_attempted += inlet->GetNumPutsAttempted();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static size_t GetNumInlets() {
      std::shared_lock lock{ registry.GetMutex() };
      return std::count_if(
        std::begin(registry), std::end(registry),
        Filter{}
      );
    }

    static double GetMeanFractionTryPutsDropped() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* inlet) {
          return accum + (
            inlet->GetNumDroppedPuts()
            / static_cast<double>( inlet->GetNumPutsAttempted() )
          );
        },
        Filter{}
      ) / GetNumInlets();
    }

    static double GetMeanFractionTryPutsThatSucceeded() {
      return 1.0 - GetMeanFractionTryPutsDropped();
    }

    static double GetMeanFractionBlockingPutsThatBlocked() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* inlet) {
          return accum + (
            inlet->GetNumPutsThatBlocked()
            / static_cast<double>( inlet->GetNumBlockingPuts() )
          );
        },
        Filter{}
      ) / GetNumInlets();
    }

    static double GetMeanFractionPutsThatSucceededEventually() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* inlet) {
          return accum + (
            inlet->GetNumPutsThatSucceededEventually()
            / static_cast<double>( inlet->GetNumPutsAttempted() )
          );
        },
        Filter{}
      ) / GetNumInlets();
    }

    static double GetMeanFractionPutsThatSucceededImmediately() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* inlet) {
          return accum + (
            inlet->GetNumPutsThatSucceededImmediately()
            / static_cast<double>( inlet->GetNumPutsAttempted() )
          );
        },
        Filter{}
      ) / GetNumInlets();
    }

    static double GetMeanRoundTripTouchesPerAttemptedPut() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* inlet) {
          return accum + (
            inlet->GetCurRoundTripTouchCount()
            / static_cast<double>( inlet->GetNumPutsAttempted() )
          );
        },
        Filter{}
      ) / GetNumInlets();
    }

    static size_t GetNumRoundTripTouches() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* inlet) {
          return accum + inlet->GetCurRoundTripTouchCount();
        },
        Filter{}
      );
    }

    // capture these from each inlet as close together as possible
    static std::string JointGet_NumTryPutsAttempted_NumDroppedPuts() {
      struct JointAdder {
        size_t num_try_puts_attempted{};
        size_t num_dropped_puts{};

        std::string pack() const {
          return uit_emp::to_string(
            num_try_puts_attempted, ',', num_dropped_puts
          );
        }

      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), JointAdder{},
        [](auto accum, const this_t* inlet) {
          accum.num_dropped_puts += inlet->GetNumDroppedPuts();
          accum.num_try_puts_attempted += inlet->GetNumTryPutsAttempted();
          return accum;
        },
        Filter{}
      ).pack();
    }

    // capture these from each inlet as close together as possible
    static std::string JointGet_NumPutsAttempted_NumRoundTripTouches() {
      struct JointAdder {
        size_t num_puts_attempted{};
        size_t num_round_trip_touches{};

        std::string pack() const {
          return uit_emp::to_string(
            num_puts_attempted, ',', num_round_trip_touches
          );
        }

      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), JointAdder{},
        [](auto accum, const this_t* inlet) {
          accum.num_puts_attempted += inlet->GetNumPutsAttempted();
          accum.num_round_trip_touches += inlet->GetCurRoundTripTouchCount();
          return accum;
        },
        Filter{}
      ).pack();
    }

    static uit_emp::DataFile MakeSummaryDataFile(const std::string& filename) {
      uit_emp::DataFile res( filename );
      res.AddFun(
        [](){
          return std::chrono::time_point_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now()
          ).time_since_epoch().count();
        },
        "Row Initial Timepoint (ns)"
      );
      res.AddVal(uitsl::get_proc_id(), "proc");
      res.AddVal(Filter::name(), "Impl Filter");
      res.AddFun(GetNumInlets, "Num Inlets");
      res.AddFun(
        JointGet_NumPutsAttempted_NumRoundTripTouches,
        "Num Puts Attempted,Num Round Trip Touches"
      );
      res.AddFun(
        JointGet_NumTryPutsAttempted_NumDroppedPuts,
        "Num Try Puts Attempted,Num Dropped Puts"
      );
      res.AddFun(GetNumBlockingPuts, "Num Blocking Puts");
      res.AddFun(GetNumTryPutsThatSucceeded, "Num Try Puts That Succeeded");
      res.AddFun(
        GetNumPutsThatSucceededEventually, "Num Puts That Succeeded Eventually"
      );
      res.AddFun(
        GetNumBlockingPutsThatSucceededImmediately,
        "Num Blocking Puts That Succeeded Immediately"
      );
      res.AddFun(
        GetNumPutsThatSucceededImmediately,
        "Num Puts That Succeeded Immediately"
      );
      res.AddFun( GetNumPutsThatBlocked, "Num Puts That Blocked" );
      res.AddFun( GetFractionTryPutsDropped, "Fraction Try Puts Dropped" );
      res.AddFun(
        GetFractionTryPutsThatSucceeded, "Fraction Try Puts That Succeeded"
      );
      res.AddFun(
        GetFractionBlockingPutsThatBlocked,
        "Fraction Blocking Puts That Blocked"
      );
      res.AddFun(
        GetFractionPutsThatSucceededEventually,
        "Fraction Puts That Succeeded Eventually"
      );
      res.AddFun(
        GetFractionPutsThatSucceededImmediately,
        "Fraction Puts That Succeeded Immediately"
      );
      res.AddFun(
        GetRoundTripTouchesPerAttemptedPut,
        "Round Trip Touches Per Attempted Put"
      );
      res.AddFun(
        GetMeanFractionTryPutsDropped,
        "Mean Fraction Try Puts Dropped"
      );
      res.AddFun(
        GetMeanFractionTryPutsThatSucceeded,
        "Mean Fraction Try Puts That Succeeded"
      );
      res.AddFun(
        GetMeanFractionBlockingPutsThatBlocked,
        "Mean Fraction Blocking Puts That Blocked"
      );
      res.AddFun(
        GetMeanFractionPutsThatSucceededEventually,
        "Mean Fraction Puts That Succeeded Eventually"
      );
      res.AddFun(
        GetMeanFractionPutsThatSucceededImmediately,
        "Mean Fraction Puts That Succeeded Immediately"
      );
      res.AddFun(
        GetMeanRoundTripTouchesPerAttemptedPut,
        "Mean Round Trip Touches Per Attempted Put"
      );
      res.AddFun(
        [](){ return uitsl::coarse_runtime<>.GetElapsed().count(); },
        "Runtime Seconds Elapsed"
      );
      res.AddFun(
        [](){
          return std::chrono::time_point_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now()
          ).time_since_epoch().count();
        },
        "Row Final Timepoint (ns)"
      );
      return res;
    }

    static auto MakeContainerDataFile(const std::string& filename) {

      auto res = uit_emp::MakeContainerDataFile<decltype(&registry)>(
        [](){ return &registry; },
        filename
      );
      res.SetFilterContainerFun( Filter{} );
      res.SetLockContainerFun( [](const auto& container_ptr){
        using mutex_t = std::decay_t<decltype(container_ptr->GetMutex())>;
        using lock_t = std::shared_lock<mutex_t>;
        return std::make_shared<lock_t>(
          container_ptr->GetMutex()
        );
      } );
      res.AddFun(
        [](){
          return std::chrono::time_point_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now()
          ).time_since_epoch().count();
        },
        "Row Initial Timepoint (ns)"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){ return inlet_ptr->WhichImplHeld(); },
        "Which Impl"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){ return inlet_ptr->GetNumPutsAttempted(); },
        "Num Puts Attempted"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){ return inlet_ptr->GetNumTryPutsAttempted(); },
        "Num Try Puts Attempted"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){ return inlet_ptr->GetNumBlockingPuts(); },
        "Num Blocking Puts"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){ return inlet_ptr->GetNumTryPutsThatSucceeded(); },
        "Num Try Puts That Succeeded"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){
          return inlet_ptr->GetNumPutsThatSucceededEventually();
        },
        "Num Puts That Succeeded Eventually"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){
          return inlet_ptr->GetNumBlockingPutsThatSucceededImmediately();
        },
        "Num Blocking Puts ThatSucceeded Immediately"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){
          return inlet_ptr->GetNumPutsThatSucceededImmediately();
        },
        "Num Puts That Succeeded Immediately"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){ return inlet_ptr->GetNumPutsThatBlocked(); },
        "Num Puts That Blocked"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){ return inlet_ptr->GetNumDroppedPuts(); },
        "Num Dropped Puts"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){ return inlet_ptr->GetFractionTryPutsDropped(); },
        "Fraction Try Puts Dropped"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){
          return inlet_ptr->GetFractionTryPutsThatSucceeded();
        },
        "Fraction Try Puts That Succeeded"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){
          return inlet_ptr->GetFractionBlockingPutsThatBlocked();
        },
        "Get Fraction Blocking Puts That Blocked"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){
          return inlet_ptr->GetFractionPutsThatSucceededEventually();
        },
        "Fraction Puts That Succeeded Eventually"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){
          return inlet_ptr->GetFractionPutsThatSucceededImmediately();
        },
        "Fraction Puts That Succeeded Immediately"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){
          return inlet_ptr->GetCurRoundTripTouchCount() / static_cast<double>(
            inlet_ptr->GetNumPutsAttempted()
          );
        },
        "Round Trip Touches Per Attempted Put"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){
          return inlet_ptr->GetCurRoundTripTouchCount();
        },
        "Num Round Trip Touches"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){ return inlet_ptr->WhichImplHeld(); },
        "Held Impl"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){
          const auto res = inlet_ptr->LookupInletProc();
          return res.has_value() ? uit_emp::to_string(*res) : "null";
        }, "Inlet Proc"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){
          const auto res = inlet_ptr->LookupOutletProc();
          return res.has_value() ? uit_emp::to_string(*res) : "null";
        }, "Outlet Proc"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){
          const auto res = inlet_ptr->LookupInletThread();
          return res.has_value() ? uit_emp::to_string(*res) : "null";
        }, "Inlet Thread"
      );
      res.AddContainerFun(
        [](const auto inlet_ptr){
          const auto res = inlet_ptr->LookupOutletThread();
          return res.has_value() ? uit_emp::to_string(*res) : "null";
        }, "Outlet Thread"
      );
      res.AddFun(
        [](){ return uitsl::coarse_runtime<>.GetElapsed().count(); },
        "Runtime Seconds Elapsed"
      );
      res.AddFun(
        [](){
          return std::chrono::time_point_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now()
          ).time_since_epoch().count();
        },
        "Row Final Timepoint (ns)"
      );
      return res;
    }

  };

  struct AllFilter {
    bool operator()( const this_t* ) const { return true; }
    static std::string_view name() { return "all"; }
  };

  struct IntraFilter {
    bool operator()( const this_t* inlet_ptr ) const {
      return inlet_ptr->HoldsIntraImpl().value();
    }
    static std::string_view name() { return "intra"; }
  };

  struct ThreadFilter {
    bool operator()( const this_t* inlet_ptr ) const {
      return inlet_ptr->HoldsThreadImpl().value();
    }
    static std::string_view name() { return "thread"; }
  };

  struct ProcFilter {
    bool operator()( const this_t* inlet_ptr ) const {
      return inlet_ptr->HoldsProcImpl().value();
    }
    static std::string_view name() { return "proc"; }
  };

  using touch_count_address_cache_t = std::optional<
    uit::impl::round_trip_touch_addr_t
  >;
  mutable touch_count_address_cache_t touch_count_address_cache{ std::nullopt };

  void DoRefreshTouchCountAddressCache() const {
    touch_count_address_cache = uit::impl::round_trip_touch_addr_t{
      *LookupMeshID(), *LookupInletNodeID(), *LookupOutletNodeID()
    };
  }

  void RefreshTouchCountAddressCacheIfNecesssary() const {
    if ( !touch_count_address_cache.has_value() )
      DoRefreshTouchCountAddressCache();
  }

  decltype(auto) GetTouchCountAddr() const {
    RefreshTouchCountAddressCacheIfNecesssary();
    return *touch_count_address_cache;
  }

  size_t GetCurRoundTripTouchCount() const {
    assert(
      uit::impl::round_trip_touch_counter.count( GetTouchCountAddr() ) &&
      "round_trip_touch_counter should be initialized during Mesh construction."
    );
    return uit::impl::round_trip_touch_counter.at( GetTouchCountAddr() );
  }

public:

  using all = RegistryAggregator<AllFilter>;
  using intra = RegistryAggregator<IntraFilter>;
  using thread = RegistryAggregator<ThreadFilter>;
  using proc = RegistryAggregator<ProcFilter>;

  /**
   * Copy constructor.
   */
  InstrumentationAggregatingInletWrapper(
    InstrumentationAggregatingInletWrapper& other
  ) : inlet( other.inlet ) {
    assert( !registry.contains(this) );
    registry.insert(this);
  }

  /**
   * Copy constructor.
   */
  InstrumentationAggregatingInletWrapper(
    const InstrumentationAggregatingInletWrapper& other
  ) : inlet( other.inlet ) {
    assert( !registry.contains(this) );
    registry.insert(this);
  };

  /**
   * Move constructor.
   */
  InstrumentationAggregatingInletWrapper(
    InstrumentationAggregatingInletWrapper&& other
  ) : inlet( std::move(other.inlet) ) {
    assert( !registry.contains(this) );
    registry.insert(this);
  };

  /**
   * Forwarding constructor.
   */
  template <typename... Args>
  explicit InstrumentationAggregatingInletWrapper(Args&&... args)
  : inlet(std::forward<Args>(args)...)
  {
    assert( !registry.contains(this) );
    registry.insert(this);
  }

  ~InstrumentationAggregatingInletWrapper() {
    [[maybe_unused]] const size_t res = registry.erase( this );
    assert( res == 1 );
  }

  void Put(const value_type& val) {
    return inlet.Put( uit::impl::RoundTripCountPacket<value_type>{
      GetCurRoundTripTouchCount() + 1, val
    } );
  }

  decltype(auto) TryPut(const value_type& val) {
    return inlet.TryPut( uit::impl::RoundTripCountPacket<value_type>{
      GetCurRoundTripTouchCount() + 1, val
    } );
  }

  template<typename P>
  decltype(auto) TryPut(P&& val) {
    return inlet.TryPut( uit::impl::RoundTripCountPacket<value_type>{
      GetCurRoundTripTouchCount() + 1, std::forward<P>(val)
    } );
  }

  decltype(auto) TryFlush() { return inlet.TryFlush(); }

  void Flush() { inlet.Flush(); }

  decltype(auto) GetNumPutsAttempted() const {
    return inlet.GetNumPutsAttempted();
  }

  decltype(auto) GetNumTryPutsAttempted() const {
    return inlet.GetNumTryPutsAttempted();
  }

  decltype(auto) GetNumBlockingPuts() const { return
    inlet.GetNumBlockingPuts();
  }

  decltype(auto) GetNumTryPutsThatSucceeded() const {
    return inlet.GetNumTryPutsThatSucceeded();
  }

  decltype(auto) GetNumPutsThatSucceededEventually() const {
    return inlet.GetNumPutsThatSucceededEventually();
  }

  decltype(auto) GetNumBlockingPutsThatSucceededImmediately() const {
    return inlet.GetNumBlockingPutsThatSucceededImmediately();
  }

  decltype(auto) GetNumPutsThatSucceededImmediately() const {
    return inlet.GetNumPutsThatSucceededImmediately();
  }

  decltype(auto) GetNumPutsThatBlocked() const {
    return inlet.GetNumPutsThatBlocked();
  }

  decltype(auto) GetNumDroppedPuts() const {
    return inlet.GetNumDroppedPuts();
  }

  decltype(auto) GetFractionTryPutsDropped() const {
    return inlet.GetFractionTryPutsDropped();
  }

  decltype(auto) GetFractionTryPutsThatSucceeded() const {
    return inlet.GetFractionTryPutsThatSucceeded();
  }

  decltype(auto) GetFractionBlockingPutsThatBlocked() const {
    return inlet.GetFractionBlockingPutsThatBlocked();
  }

  decltype(auto) GetFractionPutsThatSucceededEventually() const {
    return inlet.GetFractionPutsThatSucceededEventually();
  }

  decltype(auto) GetFractionPutsThatSucceededImmediately() const {
    return inlet.GetFractionPutsThatSucceededImmediately();
  }

  template<typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    touch_count_address_cache.reset();
    inlet.template EmplaceDuct<WhichDuct>( std::forward<Args>(args)... );
  }

  template<typename WhichDuct, typename... Args>
  void SplitDuct(Args&&... args) {
    touch_count_address_cache.reset();
    inlet.template SplitDuct<WhichDuct>( std::forward<Args>(args)... );
  }

  decltype(auto) GetDuctUID() const { return inlet.GetUID(); }

  decltype(auto) HoldsIntraImpl() const { return inlet.HoldsIntraImpl(); }

  decltype(auto) HoldsThreadImpl() const {
    return inlet.HoldsThreadImpl();
  }

  decltype(auto) HoldsProcImpl() const { return inlet.HoldsProcImpl(); }

  decltype(auto) WhichImplHeld() const { return inlet.WhichImplHeld(); }

  void RegisterInletProc(const uitsl::proc_id_t proc) const {
    inlet.RegisterInletProc(proc);
  }

  void RegisterInletThread(const uitsl::thread_id_t thread) const {
    inlet.RegisterInletThread(thread);
  }

  void RegisterOutletProc(const uitsl::proc_id_t proc) const {
    inlet.RegisterOutletProc(proc);
  }

  void RegisterOutletThread(const uitsl::thread_id_t thread) const {
    inlet.RegisterOutletThread(thread);
  }

  void RegisterEdgeID(const size_t edge_id) const {
    inlet.RegisterEdgeID(edge_id);
  }

  void RegisterInletNodeID(const size_t node_id) const {
    touch_count_address_cache.reset();
    inlet.RegisterInletNodeID(node_id);
  }

  void RegisterOutletNodeID(const size_t node_id) const {
    touch_count_address_cache.reset();
    inlet.RegisterOutletNodeID(node_id);
  }

  void RegisterMeshID(const size_t mesh_id) const {
    touch_count_address_cache.reset();
    inlet.RegisterMeshID(mesh_id);
  }

  decltype(auto) LookupOutletProc() const { return inlet.LookupOutletProc(); }

  decltype(auto) LookupOutletThread() const {
    return inlet.LookupOutletThread();
  }

  decltype(auto) LookupInletProc() const { return inlet.LookupInletProc(); }

  decltype(auto) LookupInletThread() const {
    return inlet.LookupInletThread();
  }

  decltype(auto) LookupEdgeID() const { return inlet.LookupEdgeID(); }

  decltype(auto) LookupInletNodeID() const { return inlet.LookupInletNodeID(); }

  decltype(auto) LookupOutletNodeID() const {
    return inlet.LookupOutletNodeID();
  }

  decltype(auto) LookupMeshID() const { return inlet.LookupMeshID(); }

};

} // namespace internal
} // namesapce uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_INLET_INSTRUMENTATIONAGGREGATINGINLETWRAPPER_HPP_INCLUDE
