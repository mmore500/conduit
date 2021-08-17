#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_INLET_INSTRUMENTATIONAGGREGATINGINLETWRAPPER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_INLET_INSTRUMENTATIONAGGREGATINGINLETWRAPPER_HPP_INCLUDE

#include <cstddef>
#include <shared_mutex>
#include <utility>

#include "../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../third-party/Empirical/include/emp/base/optional.hpp"

#include "../../../../uitsl/algorithm/accumulate_if.hpp"
#include "../../../../uitsl/containers/safe/unordered_set.hpp"

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

  };

  struct AllFilter {
    bool operator()( const inlet_t* ) const { return true; }
  };

  struct IntraFilter {
    bool operator()( const inlet_t* inlet ) const {
      return inlet->HoldsIntraImpl().value();
    }
  };

  struct ThreadFilter {
    bool operator()( const inlet_t* inlet ) const {
      return inlet->HoldsThreadImpl().value();
    }
  };

  struct ProcFilter {
    bool operator()( const inlet_t* inlet ) const {
      return inlet->HoldsProcImpl().value();
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

};

} // namespace internal
} // namesapce uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_INLET_INSTRUMENTATIONAGGREGATINGINLETWRAPPER_HPP_INCLUDE
