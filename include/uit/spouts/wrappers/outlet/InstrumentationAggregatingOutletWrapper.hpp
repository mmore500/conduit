#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_OUTLET_INSTRUMENTATIONAGGREGATINGOUTLETWRAPPER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_OUTLET_INSTRUMENTATIONAGGREGATINGOUTLETWRAPPER_HPP_INCLUDE

#include <algorithm>
#include <cstddef>
#include <memory>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <typeinfo>
#include <type_traits>

#include "../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../uitsl/algorithm/accumulate_if.hpp"
#include "../../../../uitsl/containers/safe/unordered_set.hpp"
#include "../../../../uitsl/countdown/coarse_runtime.hpp"
#include "../../../../uitsl/countdown/runtime.hpp"
#include "../../../../uitsl/debug/WarnOnce.hpp"
#include "../../../../uitsl/parallel/thread_utils.hpp"

namespace uit {
namespace internal {

template<typename Outlet>
class InstrumentationAggregatingOutletWrapper {

  using ImplSpec = typename Outlet::ImplSpec;

  using outlet_t = Outlet;
  outlet_t outlet;

  using value_type = typename ImplSpec::value_type;

  inline static uitsl::safe::unordered_set<const outlet_t*> registry;

  template<typename Filter>
  struct RegistryAggregator {

    static size_t GetNumReadsPerformed() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumReadsPerformed();
        },
        Filter{}
      );
    }

    static size_t GetNumReadsThatWereFresh() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumReadsThatWereFresh();
        },
        Filter{}
      );
    }

    static size_t GetNumReadsThatWereStale() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumReadsThatWereStale();
        },
        Filter{}
      );
    }

    static size_t GetNumRevisionsPulled() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumRevisionsPulled();
        },
        Filter{}
      );
    }

    static size_t GetNumTryPullsAttempted() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumTryPullsAttempted();
        },
        Filter{}
      );
    }

    static size_t GetNumBlockingPulls() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumBlockingPulls();
        },
        Filter{}
      );
    }

    static size_t GetNumBlockingPullsThatBlocked() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumBlockingPullsThatBlocked();
        },
        Filter{}
      );
    }

    static size_t GetNumRevisionsFromTryPulls() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumRevisionsFromTryPulls();
        },
        Filter{}
      );
    }

    static size_t GetNumRevisionsFromBlockingPulls() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumRevisionsFromBlockingPulls();
        },
        Filter{}
      );
    }

    static size_t GetNumPullsAttempted() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumPullsAttempted();
        },
        Filter{}
      );
    }

    static size_t GetNumPullsThatWereLadenEventually() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumPullsThatWereLadenEventually();
        },
        Filter{}
      );
    }

    static size_t GetNumBlockingPullsThatWereLadenImmediately() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumBlockingPullsThatWereLadenImmediately();
        },
        Filter{}
      );
    }

    static size_t GetNumBlockingPullsThatWereLadenEventually() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumBlockingPullsThatWereLadenEventually();
        },
        Filter{}
      );
    }

    static size_t GetNumPullsThatWereLadenImmediately() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumPullsThatWereLadenImmediately();
        },
        Filter{}
      );
    }

    static size_t GetNumTryPullsThatWereLaden() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumTryPullsThatWereLaden();
        },
        Filter{}
      );
    }

    static size_t GetNumTryPullsThatWereUnladen() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNumTryPullsThatWereUnladen();
        },
        Filter{}
      );
    }

    static double GetFractionTryPullsThatWereLaden() {
      return GetNumTryPullsThatWereLaden() / static_cast<double>(
        GetNumTryPullsAttempted()
      );
    }


    static double GetFractionTryPullsThatWereUnladen() {
      return GetNumTryPullsThatWereUnladen() / static_cast<double>(
        GetNumTryPullsAttempted()
      );
    }

    static double GetFractionBlockingPullsThatBlocked() {
      return GetNumBlockingPullsThatBlocked() / static_cast<double>(
        GetNumBlockingPulls()
      );
    }

    static double GetFractionBlockingPullsThatWereLadenImmediately() {
      return GetNumBlockingPullsThatWereLadenImmediately()
        / static_cast<double>( GetNumBlockingPulls() );
    }

    static double GetFractionBlockingPullsThatWereLadenEventually() {
      return GetNumBlockingPullsThatWereLadenEventually()
        / static_cast<double>( GetNumBlockingPulls() );
    }

    static double GetFractionPullsThatWereLadenImmediately() {
      return GetNumPullsThatWereLadenImmediately()
        / static_cast<double>( GetNumPullsAttempted() );
    }

    static double GetFractionPullsThatWereLadenEventually() {
      return GetNumPullsThatWereLadenEventually()
        / static_cast<double>( GetNumPullsAttempted() );
    }

    static size_t GetNetFluxThroughDuct() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const outlet_t* outlet) {
          return accum + outlet->GetNetFluxThroughDuct();
        },
        Filter{}
      );
    }

    static double GetFractionReadsThatWereFresh() {
      return GetNumReadsThatWereFresh() / static_cast<double>(
        GetNumReadsPerformed()
      );
    }

    static double GetFractionReadsThatWereStale() {
      return GetNumReadsThatWereStale() / static_cast<double>(
        GetNumReadsPerformed()
      );
    }

    static double GetFractionRevisionsThatWereRead() {
      return GetNumReadsThatWereFresh() / static_cast<double>(
        GetNumRevisionsPulled()
      );
    }

    static double GetFractionRevisionsThatWereNotRead() {
      return 1.0 - GetFractionRevisionsThatWereRead();
    }

    static double GetFractionDuctFluxThatWasSteppedThrough() {
      return GetNumRevisionsPulled() / static_cast<double>(
        GetNetFluxThroughDuct()
      );
    }

    static double GetFractionDuctFluxThatWasJumpedOver() {
      return 1.0 - GetFractionDuctFluxThatWasSteppedThrough();
    }

    static double GetFractionDuctFluxThatWasRead() {
      return GetNumReadsThatWereFresh() / static_cast<double>(
        GetNetFluxThroughDuct()
      );
    }

    static size_t GetNumOutlets() {
      std::shared_lock lock{ registry.GetMutex() };
      return std::count_if(
        std::begin(registry), std::end(registry),
        Filter{}
      );
    }
    static emp::DataFile MakeSummaryDataFile(const std::string& filename) {
      emp::DataFile res( filename );
      res.AddVal(uitsl::get_proc_id(), "proc");
      res.AddVal(Filter::name(), "Impl Filter");
      res.AddFun(GetNumOutlets, "Num Outlets");
      res.AddFun(GetNumReadsPerformed, "Num Reads Performed");
      res.AddFun(GetNumReadsThatWereFresh, "Num Reads That Were Fresh");
      res.AddFun(GetNumReadsThatWereStale, "Num Reads That Were Stale");
      res.AddFun(GetNumRevisionsPulled, "Num Revisions Pulled");
      res.AddFun(GetNumTryPullsAttempted, "Num Try Pulls Attempted");
      res.AddFun(GetNumBlockingPulls, "Num Blocking Pulls");
      res.AddFun(
        GetNumBlockingPullsThatBlocked, "Num Blocking Pulls That Blocked"
      );
      res.AddFun(
        GetNumRevisionsFromTryPulls, "Num Revisions From Try Pulls"
      );
      res.AddFun(
        GetNumRevisionsFromBlockingPulls, "Num Revisions From Blocking Pulls"
      );
      res.AddFun(GetNumPullsAttempted, "Num Pulls Attempted");
      res.AddFun(
        GetNumPullsThatWereLadenEventually,
        "Num Pulls That Were Laden Eventually"
      );
      res.AddFun(
        GetNumBlockingPullsThatWereLadenImmediately,
        "Num Blocking Pulls That Were Laden Immediately"
      );
      res.AddFun(
        GetNumBlockingPullsThatWereLadenEventually,
        "Num Blocking Pulls That Were Laden Eventually"
      );
      res.AddFun(
        GetNumPullsThatWereLadenImmediately,
        "Num Pulls That Were Laden Immediately"
      );
      res.AddFun(
        GetNumTryPullsThatWereLaden, "Num Try Pulls That Were Laden"
      );
      res.AddFun(
        GetNumTryPullsThatWereUnladen, "Num Try Pulls That Were Unladen"
      );
      res.AddFun(
        GetFractionTryPullsThatWereLaden, "Fraction Try Pulls That Were Laden"
      );
      res.AddFun(
        GetFractionTryPullsThatWereUnladen,
        "Fraction Try Pulls That Were Unladen"
      );
      res.AddFun(
        GetFractionBlockingPullsThatBlocked,
        "Fraction Blocking Pulls That Blocked"
      );
      res.AddFun(
        GetFractionBlockingPullsThatWereLadenImmediately,
        "Fraction Blocking Pulls That Were Laden Immediately"
      );
      res.AddFun(
        GetFractionBlockingPullsThatWereLadenEventually,
        "Fraction Blocking Pulls That Were Laden Eventually"
      );
      res.AddFun(
        GetFractionPullsThatWereLadenImmediately,
        "Fraction Pulls That Were Laden Immediately"
      );
      res.AddFun(
        GetFractionPullsThatWereLadenEventually,
        "Fraction Pulls That Were Laden Eventually"
      );
      res.AddFun(GetNetFluxThroughDuct, "Net Flux Through Duct");
      res.AddFun(
        GetFractionReadsThatWereFresh, "Fraction Reads That Were Fresh"
      );
      res.AddFun(
        GetFractionReadsThatWereStale,
        "Fraction Reads That Were Stale"
      );
      res.AddFun(
        GetFractionRevisionsThatWereRead,
        "Fraction Revisions That Were Read"
      );
      res.AddFun(
        GetFractionRevisionsThatWereNotRead,
        "Fraction Revisions That Were Not Read"
      );
      res.AddFun(
        GetFractionDuctFluxThatWasSteppedThrough,
        "Fraction Duct Flux That Was Stepped Through"
      );
      res.AddFun(
        GetFractionDuctFluxThatWasJumpedOver,
        "Fraction Duct Flux That Was Jumped Over"
      );
      res.AddFun(
        GetFractionDuctFluxThatWasRead,
        "Fraction Duct Flux That Was Read"
      );
      res.AddFun(
        [](){ return uitsl::runtime<>.GetElapsed().count(); },
        "Runtime Seconds"
      );
      return res;
    }

    static auto MakeContainerDataFile(const std::string& filename) {

      auto res = emp::MakeContainerDataFile<decltype(&registry)>(
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
      res.AddContainerFun(
        [](const auto outlet_ptr){ return outlet_ptr->WhichImplHeld(); },
        "Which Impl"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){ return outlet_ptr->GetNumReadsPerformed(); },
        "Num Reads Performed"
      );
       res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumReadsThatWereFresh();
        },
        "Num Reads That Were Fresh"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumReadsThatWereStale();
        },
        "Num Reads That Were Stale"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumRevisionsPulled();
        },
        "Num Revisions Pulled"
      );
       res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumTryPullsAttempted();
        },
        "Num Try Pulls Attempted"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){ return outlet_ptr->GetNumBlockingPulls(); },
        "Num Blocking Pulls"
      );
       res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumBlockingPullsThatBlocked();
        },
        "Num Blocking Pulls That Blocked"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumRevisionsFromTryPulls();
        },
        "Num Revisions From Try Pulls"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumRevisionsFromBlockingPulls();
        },
        "Num Revisions From Blocking Pulls"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumPullsAttempted();
        },
        "Num Pulls Attempted"
      );
       res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumPullsThatWereLadenEventually();
        },
        "Num Pulls That Were Laden Eventually"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumBlockingPullsThatWereLadenImmediately();
        },
        "Num Blocking Pulls That Were Laden Immediately"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumBlockingPullsThatWereLadenEventually();
        },
        "Num Blocking Pulls That Were Laden Eventually"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumPullsThatWereLadenImmediately();
        },
        "Num Pulls That Were Laden Immediately"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){ return outlet_ptr->GetNumTryPullsThatWereLaden(); },
        "Num Try Pulls That Were Laden"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumTryPullsThatWereUnladen();
        },
        "Num Try Pulls That Were Unladen"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionTryPullsThatWereLaden();
        },
        "Fraction Try Pulls That Were Laden"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionTryPullsThatWereUnladen();
        },
        "Fraction Try Pulls That Were Unladen"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionBlockingPullsThatBlocked();
        },
        "Fraction Blocking Pulls That Blocked "
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionBlockingPullsThatWereLadenImmediately();
        },
        "Fraction Blocking Pulls That Were Laden Immediately"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionPullsThatWereLadenImmediately();
        },
        "Fraction Pulls That Were Laden Immediately"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionPullsThatWereLadenEventually();
        },
        "Fraction Pulls That Were Laden Eventually"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNetFluxThroughDuct();
        },
        "Net Flux Through Duct "
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionReadsThatWereFresh();
        },
        "Fraction Reads That Were Fresh "
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionReadsThatWereStale();
        },
        "Fraction Reads That Were Stale "
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionRevisionsThatWereRead();
        },
        "Fraction Revisions That Were Read "
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionRevisionsThatWereNotRead();
        },
        "Fraction Revisions That Were Not Read"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionDuctFluxThatWasSteppedThrough();
        },
        "Fraction Duct Flux That Was Stepped Through"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionDuctFluxThatWasJumpedOver();
        },
        "Fraction Duct Flux That Was Jumped Over"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionDuctFluxThatWasRead();
        },
        "Fraction Duct Flux That Was Read"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){ return outlet_ptr->WhichImplHeld(); },
        "Held Impl"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          const auto res = outlet_ptr->LookupInletProc();
          return res.has_value() ? emp::to_string(*res) : "null";
        }, "Inlet Proc"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          const auto res = outlet_ptr->LookupOutletProc();
          return res.has_value() ? emp::to_string(*res) : "null";
        }, "Outlet Proc"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          const auto res = outlet_ptr->LookupInletThread();
          return res.has_value() ? emp::to_string(*res) : "null";
        }, "Inlet Thread"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          const auto res = outlet_ptr->LookupOutletThread();
          return res.has_value() ? emp::to_string(*res) : "null";
        }, "Outlet Thread"
      );
      res.AddFun(
        [](){ return uitsl::coarse_runtime<>.GetElapsed().count(); },
        "Runtime Seconds"
      );
      return res;
    }

  };

  struct AllFilter {
    bool operator()( const outlet_t* ) const { return true; }
    static std::string name() { return "all"; }
  };

  struct IntraFilter {
    bool operator()( const outlet_t* outlet_ptr ) const {
      return outlet_ptr->HoldsIntraImpl().value();
    }
    static std::string_view name() { return "intra"; }
  };

  struct ThreadFilter {
    bool operator()( const outlet_t* outlet_ptr ) const {
      return outlet_ptr->HoldsThreadImpl().value();
    }
    static std::string_view name() { return "thread"; }
  };

  struct ProcFilter {
    bool operator()( const outlet_t* outlet_ptr ) const {
      return outlet_ptr->HoldsProcImpl().value();
    }
    static std::string_view name() { return "proc"; }
  };


public:

  using all = RegistryAggregator<AllFilter>;
  using intra = RegistryAggregator<IntraFilter>;
  using thread = RegistryAggregator<ThreadFilter>;
  using proc = RegistryAggregator<ProcFilter>;

  /**
   * Copy constructor.
   */
  InstrumentationAggregatingOutletWrapper(
    InstrumentationAggregatingOutletWrapper& other
  ) : outlet( other.outlet ) {
    emp_assert( !registry.contains(&outlet) );
    registry.insert(&outlet);
  }

  /**
   * Copy constructor.
   */
  InstrumentationAggregatingOutletWrapper(
    const InstrumentationAggregatingOutletWrapper& other
  ) : outlet( other.outlet ) {
    emp_assert( !registry.contains(&outlet) );
    registry.insert(&outlet);
  };

  /**
   * Move constructor.
   */
  InstrumentationAggregatingOutletWrapper(
    InstrumentationAggregatingOutletWrapper&& other
  ) : outlet( std::move(other.outlet) ) {
    emp_assert( !registry.contains(&outlet) );
    registry.insert(&outlet);
  };

  /**
   * Forwarding constructor.
   */
  template <typename... Args>
  InstrumentationAggregatingOutletWrapper(Args&&... args)
  : outlet(std::forward<Args>(args)...) {
    emp_assert( !registry.contains(&outlet) );
    registry.insert(&outlet);
  }

  ~InstrumentationAggregatingOutletWrapper() {
    [[maybe_unused]] const size_t res = registry.erase( &outlet );
    emp_assert( res == 1, res );
  }

  decltype(auto) TryStep(const size_t num_steps) {
    return outlet.TryStep( num_steps );
  }

  decltype(auto) Jump() { return outlet.Jump(); }

  decltype(auto) Get() const { return outlet.Get(); }

  decltype(auto) Get() { return outlet.Get(); }

  decltype(auto) JumpGet() { return outlet.JumpGet(); }

  decltype(auto) GetNext() { return outlet.GetNext(); }

  decltype(auto) GetNextOrNullopt() { return outlet.GetNextOrNullopt(); }

  decltype(auto) GetNumReadsPerformed() const {
    return outlet.GetNumReadsPerformed();
  }

  decltype(auto) GetNumReadsThatWereFresh() const {
    return outlet.GetNumReadsThatWereFresh();
  }

  decltype(auto) GetNumReadsThatWereStale() const {
    return outlet.GetNumReadsThatWereStale();
  }

  decltype(auto) GetNumRevisionsPulled() const {
    return outlet.GetNumRevisionsPulled();
  }

  decltype(auto) GetNumTryPullsAttempted() const {
    return outlet.GetNumTryPullsAttempted();
  }

  decltype(auto) GetNumBlockingPulls() const {
    return outlet.GetNumBlockingPulls();
  }

  decltype(auto) GetNumBlockingPullsThatBlocked() const {
    return outlet.GetNumBlockingPullsThatBlocked();
  }

  decltype(auto) GetNumRevisionsFromTryPulls() const {
    return outlet.GetNumRevisionsFromTryPulls();
  }

  decltype(auto) GetNumRevisionsFromBlockingPulls() const {
    return outlet.GetNumRevisionsFromBlockingPulls();
  }

  decltype(auto) GetNumPullsAttempted() const {
    return outlet.GetNumPullsAttempted();
  }

  decltype(auto) GetNumPullsThatWereLadenEventually() const {
    return outlet.GetNumPullsThatWereLadenEventually();
  }

  decltype(auto) GetNumBlockingPullsThatWereLadenImmediately() const {
    return outlet.GetNumBlockingPullsThatWereLadenImmediately();
  }

  decltype(auto) GetNumBlockingPullsThatWereLadenEventually() const {
    return outlet.GetNumBlockingPullsThatWereLadenEventually();
  }

  decltype(auto) GetNumPullsThatWereLadenImmediately() const {
    return outlet.GetNumPullsThatWereLadenImmediately();
  }

  decltype(auto) GetNumTryPullsThatWereLaden() const {
    return outlet.GetNumTryPullsThatWereLaden();
  }

  decltype(auto) GetNumTryPullsThatWereUnladen() const {
    return outlet.GetNumTryPullsThatWereUnladen();
  }

  decltype(auto) GetFractionTryPullsThatWereLaden() const {
    return outlet.GetFractionTryPullsThatWereLaden();
  }

  decltype(auto) GetFractionTryPullsThatWereUnladen() const {
    return outlet.GetFractionTryPullsThatWereUnladen();
  }

  decltype(auto) GetFractionBlockingPullsThatBlocked() const {
    return outlet.GetFractionBlockingPullsThatBlocked();
  }

  decltype(auto) GetFractionBlockingPullsThatWereLadenImmediately() const {
    return outlet.GetFractionBlockingPullsThatWereLadenImmediately();
  }

  decltype(auto) GetFractionPullsThatWereLadenImmediately() const {
    return outlet.GetFractionPullsThatWereLadenImmediately();
  }

  decltype(auto) GetFractionPullsThatWereLadenEventually() const {
    return outlet.GetFractionPullsThatWereLadenEventually();
  }

  decltype(auto) GetNetFluxThroughDuct() const {
    return outlet.GetNetFluxThroughDuct();
  }

  decltype(auto) GetFractionReadsThatWereFresh() const {
    return outlet.GetFractionReadsThatWereFresh();
  }

  decltype(auto) GetFractionRevisionsThatWereRead() const {
    return outlet.GetFractionRevisionsThatWereRead();
  }

  decltype(auto) GetFractionRevisionsThatWereNotRead() const {
    return outlet.GetFractionRevisionsThatWereNotRead();
  }

  decltype(auto) GetFractionDuctFluxThatWasSteppedThrough() const {
    return outlet.GetFractionDuctFluxThatWasSteppedThrough();
  }

  decltype(auto) GetFractionDuctFluxThatWasJumpedOver() const {
    return outlet.GetFractionDuctFluxThatWasJumpedOver();
  }

  decltype(auto) GetFractionDuctFluxThatWasRead() const {
    return outlet.GetFractionDuctFluxThatWasRead();
  }

  template <typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    outlet.template EmplaceDuct<WhichDuct>( std::forward<Args>(args)... );
  }

  template <typename WhichDuct, typename... Args>
  void SplitDuct(Args&&... args) {
    outlet.template SplitDuct<WhichDuct>( std::forward<Args>(args)... );
  }

  decltype(auto) GetDuctUID() const { return outlet.GetUID(); }

  decltype(auto) HoldsIntraImpl() const { return outlet.HoldsIntraImpl(); }

  decltype(auto) HoldsThreadImpl() const { return outlet.HoldsThreadImpl(); }

  decltype(auto) HoldsProcImpl() const { return outlet.HoldsProcImpl(); }

  decltype(auto) WhichImplHeld() const { return outlet.WhichImplHeld(); }

  decltype(auto) CanStep() const { return outlet.CanStep(); }

  void RegisterInletProc(const uitsl::proc_id_t proc) const {
    outlet.RegisterInletProc(proc);
  }

  void RegisterInletThread(const uitsl::thread_id_t thread) const {
    outlet.RegisterInletThread(thread);
  }

  void RegisterOutletProc(const uitsl::proc_id_t proc) const {
    outlet.RegisterOutletProc(proc);
  }

  void RegisterOutletThread(const uitsl::thread_id_t thread) const {
    outlet.RegisterOutletThread(thread);
  }

  void RegisterEdgeID(const size_t edge_id) const {
    outlet.RegisterEdgeID(edge_id);
  }

  void RegisterInletNodeID(const size_t node_id) const {
    outlet.RegisterInletNodeID(node_id);
  }

  void RegisterOutletNodeID(const size_t node_id) const {
    outlet.RegisterOutletNodeID(node_id);
  }

  decltype(auto) LookupOutletProc() const { return outlet.LookupOutletProc(); }

  decltype(auto) LookupOutletThread() const {
    return outlet.LookupOutletThread();
  }

  decltype(auto) LookupInletProc() const { return outlet.LookupInletProc(); }

  decltype(auto) LookupInletThread() const {
    return outlet.LookupInletThread();
  }

  decltype(auto) LookupEdgeID() const { return outlet.LookupEdgeID(); }

};

} // namespace internal
} // namespace uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_OUTLET_INSTRUMENTATIONAGGREGATINGOUTLETWRAPPER_HPP_INCLUDE
