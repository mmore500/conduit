#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_INLET_CACHINGINLETWRAPPER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_INLET_CACHINGINLETWRAPPER_HPP_INCLUDE

#include <cstddef>
#include <utility>

#include "../../../../uit_emp/datastructs/QueueCache.hpp"

#include "../../../../uitsl/distributed/CachePacket.hpp"
#include "../../../../uitsl/mpi/proc_id_t.hpp"
#include "../../../../uitsl/parallel/thread_utils.hpp"

namespace uit {
namespace internal {

template<typename Inlet>
class CachingInletWrapper {

  size_t uid_stepper{ 1 };

  using ImplSpec = typename Inlet::ImplSpec;

  using inlet_t = Inlet;
  inlet_t inlet;

  using value_type = typename ImplSpec::value_type;

  uit_emp::QueueCache<
    value_type,
    size_t,
    ImplSpec::SpoutCacheSize
  > cache;

  void DoProcPut(const value_type& val) {
    bool hit{ true };

    const size_t uid = cache.Get(
      val,
      [this, &hit](const auto& key){ hit = false; return uid_stepper++; }
    );

    if ( hit ) inlet.Put( uitsl::CachePacket<value_type>{ uid } );
    else inlet.Put( uitsl::CachePacket<value_type>{ uid, val } );
  }

  bool DoProcTryPut(const value_type& val) {
    bool hit{ true };

    const size_t uid = cache.Get(
      val,
      [this, &hit](const auto& key){ hit = false; return uid_stepper++; }
    );

    if ( hit ) return inlet.TryPut( uitsl::CachePacket<value_type>{ uid } );
    else {
      const bool res{
        inlet.TryPut( uitsl::CachePacket<value_type>{ uid, val } )
      };
      if ( !res ) cache.Delete( val );
      return res;
    }
  }

  template<typename P>
  bool DoProcTryPut(P&& val) {
    bool hit{ true };

    const size_t uid = cache.Get(
      val,
      [this, &hit](const auto& key){ hit = false; return uid_stepper++; }
    );

    if ( hit ) return inlet.TryPut( uitsl::CachePacket<value_type>{ uid } );
    else {
      const bool res{ inlet.TryPut(
        uitsl::CachePacket<value_type>{ uid, std::forward<P>(val) }
      ) };
      if ( !res ) cache.Delete( val );
      return res;
    }
  }

public:

  /**
   * Copy constructor.
   */
  CachingInletWrapper(CachingInletWrapper& other) = default;

  /**
   * Copy constructor.
   */
  CachingInletWrapper(const CachingInletWrapper& other) = default;

  /**
   * Move constructor.
   */
  CachingInletWrapper(CachingInletWrapper&& other) = default;

  /**
   * Forwarding constructor.
   */
  template <typename... Args>
  CachingInletWrapper(Args&&... args)
  : inlet(std::forward<Args>(args)...)
  { ; }

  void Put(const value_type& val) {
    if ( HoldsProcImpl().value_or(true) ) DoProcPut( val );
    else inlet.Put( uitsl::CachePacket<value_type>{ 0, val } );
  }

  bool TryPut(const value_type& val) {
    if ( HoldsProcImpl().value_or(true) ) return DoProcTryPut( val );
    else return inlet.TryPut( uitsl::CachePacket<value_type>{ 0, val } );
  }

  template<typename P>
  bool TryPut(P&& val) {
    if ( HoldsProcImpl().value_or(true) ) return DoProcTryPut(
      std::forward<P>(val)
    );
    else return inlet.TryPut(
      uitsl::CachePacket<value_type>{ 0, std::forward<P>(val) }
    );

  }

  decltype(auto) TryFlush() { return inlet.TryFlush(); }

  void Flush() { inlet.Flush(); }

  decltype(auto) GetNumPutsAttempted() const { return inlet.GetNumPutsAttempted(); }

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

  auto GetDuctUID() const { return inlet.GetUID(); }

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
    inlet.RegisterInletNodeID(node_id);
  }

  void RegisterOutletNodeID(const size_t node_id) const {
    inlet.RegisterOutletNodeID(node_id);
  }

  void RegisterMeshID(const size_t mesh_id) const {
    inlet.RegisterMeshID(mesh_id);
  }

  decltype(auto) LookupOutletProc() const { return inlet.LookupOutletProc(); }

  decltype(auto) LookupOutletThread() const {
    return inlet.LookupOutletThread();
  }

  decltype(auto) LookupInletProc() const { return inlet.LookupInletProc(); }

  decltype(auto) LookupInletThread() const { return inlet.LookupInletThread(); }

  decltype(auto) LookupEdgeID() const { return inlet.LookupEdgeID(); }

  decltype(auto) LookupInletNodeID() const { return inlet.LookupInletNodeID(); }

  decltype(auto) LookupOutletNodeID() const {
    return inlet.LookupOutletNodeID();
  }

  decltype(auto) LookupMeshID() const { return inlet.LookupMeshID(); }

};

} // namespace internal
} // namesapce uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_INLET_CACHINGINLETWRAPPER_HPP_INCLUDE
