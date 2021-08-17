#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_INLET_INSTRUMENTATIONAGGREGATINGINLETWRAPPER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_INLET_INSTRUMENTATIONAGGREGATINGINLETWRAPPER_HPP_INCLUDE

#include <cstddef>
#include <memory>
#include <shared_mutex>
#include <string>
#include <utility>

#include "../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../uitsl/algorithm/accumulate_if.hpp"
#include "../../../../uitsl/countdown/runtime.hpp"
#include "../../../../uitsl/containers/safe/unordered_set.hpp"
#include "../../../../uitsl/mpi/comm_utils.hpp"

namespace uit {
namespace internal {

template<typename Inlet>
class InstrumentationAggregatingInletWrapper {

  using ImplSpec = typename Inlet::ImplSpec;

  using inlet_t = Inlet;
  inlet_t inlet;

  using value_type = typename ImplSpec::value_type;

  inline static uitsl::safe::unordered_set<const inlet_t*> registry;

  template<typename Filter>
  struct RegistryAggregator {

    static size_t GetNumPutsAttempted() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const inlet_t* inlet) {
          return accum + inlet->GetNumPutsAttempted();
        },
        Filter{}
      );
    }

    static size_t GetNumTryPutsAttempted() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const inlet_t* inlet) {
          return accum + inlet->GetNumTryPutsAttempted();
        },
        Filter{}
      );
    }

    static size_t GetNumBlockingPuts() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const inlet_t* inlet) {
          return accum + inlet->GetNumBlockingPuts();
        },
        Filter{}
      );
    }

    static size_t GetNumTryPutsThatSucceeded() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const inlet_t* inlet) {
          return accum + inlet->GetNumTryPutsThatSucceeded();
        },
        Filter{}
      );
    }

    static size_t GetNumPutsThatSucceededEventually()  {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const inlet_t* inlet) {
          return accum + inlet->GetNumTryPutsAttempted();
        },
        Filter{}
      );
    }

  static size_t GetNumBlockingPutsThatSucceededImmediately() {
    std::shared_lock lock{ registry.GetMutex() };
    return uitsl::accumulate_if(
      std::begin(registry), std::end(registry), size_t{},
      [](size_t accum, const inlet_t* inlet) {
        return accum + inlet->GetNumBlockingPutsThatSucceededImmediately();
      },
      Filter{}
    );
  }

    static size_t GetNumPutsThatSucceededImmediately() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const inlet_t* inlet) {
          return accum + inlet->GetNumPutsThatSucceededImmediately();
        },
        Filter{}
      );
    }

    static size_t GetNumPutsThatBlocked() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const inlet_t* inlet) {
          return accum + inlet->GetNumPutsThatBlocked();
        },
        Filter{}
      );
    }

    static size_t GetNumDroppedPuts() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const inlet_t* inlet) {
          return accum + inlet->GetNumDroppedPuts();
        },
        Filter{}
      );
    }

    static double GetFractionTryPutsDropped() {
      return GetNumDroppedPuts() / static_cast<double>(
        GetNumTryPutsAttempted()
      );
    }

    static double GetFractionTryPutsThatSucceeded() {
      return 1.0 - GetFractionTryPutsDropped();
    }

    static double GetFractionBlockingPutsThatBlocked() {
      return GetNumPutsThatBlocked() / static_cast<double>(
        GetNumBlockingPuts()
      );
    }

    static double GetFractionPutsThatSucceededEventually() {
      return GetNumPutsThatSucceededEventually() / static_cast<double>(
        GetNumPutsAttempted()
      );
    }

    static double GetFractionPutsThatSucceededImmediately() {
      return GetNumPutsThatSucceededImmediately() / static_cast<double>(
        GetNumPutsAttempted()
      );
    }

    static emp::DataFile MakeSummaryDataFile(const std::string& filename) {
      emp::DataFile res( filename );
      res.AddVal(uitsl::get_proc_id(), "proc");
      res.AddVal(Filter::name(), "Impl Filter");
      res.AddFun(GetNumPutsAttempted, "Num Puts Attempted");
      res.AddFun(GetNumTryPutsAttempted, "Num Try Puts Attempted");
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
      res.AddFun( GetNumDroppedPuts, "Num Dropped Puts" );
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
        [](){ return uitsl::runtime<>.GetElapsed(); }, "Runtime Seconds"
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
        const std::shared_ptr<void> res = std::make_shared<std::shared_lock>(
          container_ptr->GetMutex()
        );
        return res;
      } );
      res.AddContainerFun(
        [](const auto& inlet){ return inlet.GetNumPutsAttempted(); },
        "Which Impl"
      );
      res.AddContainerFun(
        [](const auto& inlet){ return inlet.GetNumPutsAttempted(); },
        "Num Puts Attempted"
      );
      res.AddContainerFun(
        [](const auto& inlet){ return inlet.GetNumTryPutsAttempted(); },
        "Num Try Puts Attempted"
      );
      res.AddContainerFun(
        [](const auto& inlet){ return inlet.GetNumBlockingPuts(); },
        "Num Blocking Puts"
      );
      res.AddContainerFun(
        [](const auto& inlet){ return inlet.GetNumTryPutsThatSucceeded(); },
        "Num Try Puts That Succeeded"
      );
      res.AddContainerFun(
        [](const auto& inlet){
          return inlet.GetNumPutsThatSucceededEventually();
        },
        "Num Puts That Succeeded Eventually"
      );
      res.AddContainerFun(
        [](const auto& inlet){
          return inlet.GetNumBlockingPutsThatSucceededImmediately();
        },
        "Num Blocking Puts ThatSucceeded Immediately"
      );
      res.AddContainerFun(
        [](const auto& inlet){
          return inlet.GetNumPutsThatSucceededImmediately();
        },
        "Num Puts That Succeeded Immediately"
      );
      res.AddContainerFun(
        [](const auto& inlet){ return inlet.GetNumPutsThatBlocked(); },
        "Num Puts That Blocked"
      );
      res.AddContainerFun(
        [](const auto& inlet){ return inlet.GetNumDroppedPuts(); },
        "Num Dropped Puts"
      );
      res.AddContainerFun(
        [](const auto& inlet){ return inlet.GetFractionTryPutsDropped(); },
        "Fraction Try Puts Dropped"
      );
      res.AddContainerFun(
        [](const auto& inlet){
          return inlet.GetFractionTryPutsThatSucceeded();
        },
        "Fraction Try Puts That Succeeded"
      );
      res.AddContainerFun(
        [](const auto& inlet){
          return inlet.GetFractionBlockingPutsThatBlocked();
        },
        "Get Fraction Blocking Puts That Blocked"
      );
      res.AddContainerFun(
        [](const auto& inlet){
          return inlet.GetFractionPutsThatSucceededEventually();
        },
        "Fraction Puts That Succeeded Eventually"
      );
      res.AddContainerFun(
        [](const auto& inlet){
          return inlet.GetFractionPutsThatSucceededImmediately();
        },
        "Fraction Puts That Succeeded Immediately"
      );
      res.AddContainerFun(
        [](const auto& inlet){ return inlet.GetHeldImpl(); }, "Held Impl"
      );
      res.AddContainerFun(
        [](const auto& inlet){
          const auto res = inlet.LookupInletProc();
          return res.has_value() ? emp::to_string(*res) : "null";
        }, "Inlet Proc"
      );
      res.AddContainerFun(
        [](const auto& inlet){
          const auto res = inlet.LookupOutletProc();
          return res.has_value() ? emp::to_string(*res) : "null";
        }, "Outlet Proc"
      );
      res.AddContainerFun(
        [](const auto& inlet){
          const auto res = inlet.LookupInletThread();
          return res.has_value() ? emp::to_string(*res) : "null";
        }, "Inlet Thread"
      );
      res.AddContainerFun(
        [](const auto& inlet){
          const auto res = inlet.LookupOutletThread();
          return res.has_value() ? emp::to_string(*res) : "null";
        }, "Outlet Thread"
      );
      res.AddFun(
        [](){ return uitsl::runtime<>.GetElapsed(); },
        "Runtime Seconds"
      );
      return res;
    }

  };

  struct AllFilter {
    bool operator()( const inlet_t* ) const { return true; }
    static const std::string& name() { return "all"; }
  };

  struct IntraFilter {
    bool operator()( const inlet_t* inlet ) const {
      return inlet->HoldsIntraImpl().value();
    }
    static const std::string& name() { return "intra"; }
  };

  struct ThreadFilter {
    bool operator()( const inlet_t* inlet ) const {
      return inlet->HoldsThreadImpl().value();
    }
    static const std::string& name() { return "thread"; }
  };

  struct ProcFilter {
    bool operator()( const inlet_t* inlet ) const {
      return inlet->HoldsProcImpl().value();
    }
    static const std::string& name() { return "proc"; }
  };


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
  ) {
    inlet = other.inlet;
    emp_assert( !registry.contains(&inlet) );
    registry.insert(&inlet);
  }

  /**
   * Copy constructor.
   */
  InstrumentationAggregatingInletWrapper(
    const InstrumentationAggregatingInletWrapper& other
  ) {
    inlet = other.inlet;
    emp_assert( !registry.contains(&inlet) );
    registry.insert(&inlet);
  };

  /**
   * Move constructor.
   */
  InstrumentationAggregatingInletWrapper(
    InstrumentationAggregatingInletWrapper&& other
  ) {
    inlet = std::move(other.inlet);
    emp_assert( !registry.contains(&inlet) );
    registry.insert(&inlet);
  };

  /**
   * Forwarding constructor.
   */
  template <typename... Args>
  InstrumentationAggregatingInletWrapper(Args&&... args)
  : inlet(std::forward<Args>(args)...)
  {
    emp_assert( !registry.contains(&inlet) );
    registry.insert(&inlet);
  }

  ~InstrumentationAggregatingInletWrapper() {
    const size_t res = registry.erase( &inlet );
    emp_assert( res == 1, res );
  }

  void Put(const value_type& val) { return inlet.Put(val); }

  decltype(auto) TryPut(const value_type& val) { return inlet.TryPut(val); }

  template<typename P>
  decltype(auto) TryPut(P&& val) {
    return inlet.TryPut( std::forward<P>(val) );
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
    inlet.template EmplaceDuct<WhichDuct>( std::forward<Args>(args)... );
  }

  template<typename WhichDuct, typename... Args>
  void SplitDuct(Args&&... args) {
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

  decltype(auto) LookupOutletProc() const { return inlet.LookupOutletProc(); }

  decltype(auto) LookupOutletThread() const {
    return inlet.LookupOutletThread();
  }

  decltype(auto) LookupInletProc() const { return inlet.LookupInletProc(); }

  decltype(auto) LookupInletThread() const {
    return inlet.LookupInletThread();
  }

};

} // namespace internal
} // namesapce uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_INLET_INSTRUMENTATIONAGGREGATINGINLETWRAPPER_HPP_INCLUDE
