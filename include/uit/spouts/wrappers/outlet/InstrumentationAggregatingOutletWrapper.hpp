#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_OUTLET_INSTRUMENTATIONAGGREGATINGOUTLETWRAPPER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_OUTLET_INSTRUMENTATIONAGGREGATINGOUTLETWRAPPER_HPP_INCLUDE

#include <cstddef>
#include <shared_mutex>
#include <string>
#include <typeinfo>

#include "../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../third-party/Empirical/include/emp/base/optional.hpp"

#include "../../../../uitsl/algorithm/accumulate_if.hpp"
#include "../../../../uitsl/containers/safe/unordered_set.hpp"
#include "../../../../uitsl/debug/WarnOnce.hpp"

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

  };

  struct AllFilter {
    bool operator()( const outlet_t* ) const { return true; }
  };

  struct IntraFilter {
    bool operator()( const outlet_t* outlet ) const {
      return outlet->HoldsIntraImpl().value();
    }
  };

  struct ThreadFilter {
    bool operator()( const outlet_t* outlet ) const {
      return outlet->HoldsThreadImpl().value();
    }
  };

  struct ProcFilter {
    bool operator()( const outlet_t* outlet ) const {
      return outlet->HoldsProcImpl().value();
    }
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
  ) {
    outlet = other.outlet;
    emp_assert( !registry.contains(&outlet) );
    registry.insert(&outlet);
  }

  /**
   * Copy constructor.
   */
  InstrumentationAggregatingOutletWrapper(
    const InstrumentationAggregatingOutletWrapper& other
  ) {
    outlet = other.outlet;
    emp_assert( !registry.contains(&outlet) );
    registry.insert(&outlet);
  };

  /**
   * Move constructor.
   */
  InstrumentationAggregatingOutletWrapper(
    InstrumentationAggregatingOutletWrapper&& other
  ) {
    outlet = std::move(other.outlet);
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

  ~InstrumentationAggregatingOutletWrapper(){
    const size_t res = registry.erase( &outlet );
    emp_assert( res == 1, res );
  }

  decltype(auto) TryStep(const size_t num_steps) {
    return outlet.TryStep( num_steps );
  }

  decltype(auto) Jump() { outlet.Jump(); }

  decltype(auto) Get() const { outlet.Get(); }

  decltype(auto) Get() { outlet.Get(); }

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

  decltype(auto) CanStep() const { return outlet.CanStep(); }

};

} // namespace internal
} // namespace uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_OUTLET_INSTRUMENTATIONAGGREGATINGOUTLETWRAPPER_HPP_INCLUDE
