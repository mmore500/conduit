#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_OUTLET_CACHINGOUTLETWRAPPER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_OUTLET_CACHINGOUTLETWRAPPER_HPP_INCLUDE

#include <cstddef>
#include <optional>
#include <string>
#include <typeinfo>

#include "../../../../uit_emp/datastructs/QueueCache.hpp"

#include "../../../../uitsl/debug/WarnOnce.hpp"
#include "../../../../uitsl/distributed/CachePacket.hpp"
#include "../../../../uitsl/mpi/proc_id_t.hpp"
#include "../../../../uitsl/parallel/thread_utils.hpp"

namespace uit {
namespace internal {

template<typename Outlet>
class CachingOutletWrapper {

  Outlet outlet;

  using ImplSpec = typename Outlet::ImplSpec;

  using this_t = CachingOutletWrapper<Outlet>;
  using value_type = typename ImplSpec::value_type;

  uit_emp::QueueCache<
    size_t,
    value_type,
    ImplSpec::SpoutCacheSize
  > cache;

  void CacheCurrent() {
    const auto& packet = outlet.Get();
    if ( packet.HasData() ) cache.Put( packet.GetID(), packet.GetData() );
    else if ( cache.Contains(packet.GetID()) ) cache.Get( packet.GetID() );
    //^ Get moves item to the front of the cache
    else {
      static const uitsl::WarnOnce w{ "missing cache packet" };
    }
  }

  size_t DoProcTryStep(const size_t num_steps=1) {

    size_t num_steps_countdown{ num_steps };

    while ( num_steps_countdown && outlet.TryStep( 1 ) ) {
      --num_steps_countdown;
      CacheCurrent();
    }

    return num_steps - num_steps_countdown;
  }

  size_t DoProcJump() {
    if ( CanStep() ) return DoProcTryStep( std::numeric_limits<size_t>::max() );
    else {
      const size_t res{ outlet.Jump() };
      CacheCurrent();
      return res;
    }
  }

  const value_type& DoProcGet() const {
    const static value_type pristine{};
    if ( cache.Empty() ) return pristine;
    else return cache.begin()->second;
  }

  value_type& DoProcGet() {
    const static uitsl::WarnOnce w{
      std::string{}
      + "Calling non-const Get on CachingOutletWrapper incurs extra copy, T "
      + typeid( value_type ).name()
      + " ... consider using std::as_const"
    };

    thread_local value_type mutable_copy;
    mutable_copy = const_cast<const this_t*>(this)->DoProcGet();
    return mutable_copy;
  }


public:

  /**
   * Copy constructor.
   */
  CachingOutletWrapper(CachingOutletWrapper& other) = default;

  /**
   * Copy constructor.
   */
  CachingOutletWrapper(const CachingOutletWrapper& other) = default;

  /**
   * Move constructor.
   */
  CachingOutletWrapper(CachingOutletWrapper&& other) = default;

  /**
   * Forwarding constructor.
   */
  template <typename... Args>
  CachingOutletWrapper(Args&&... args)
  : outlet(std::forward<Args>(args)...)
  { ; }

  size_t TryStep(const size_t num_steps) {
    if ( HoldsProcImpl().value_or( true ) ) return DoProcTryStep( num_steps );
    else return outlet.TryStep( num_steps );
  }

  size_t Jump() {
    if ( HoldsProcImpl().value_or( true ) ) return DoProcJump();
    else return outlet.Jump();
  }

  const value_type& Get() const {
    if ( HoldsProcImpl().value_or( true ) ) return DoProcGet();
    else return outlet.Get().GetData();
  }

  value_type& Get() {
    if ( HoldsProcImpl().value_or( true ) ) return DoProcGet();
    else return outlet.Get().GetData();
  }

  const value_type& JumpGet() { Jump(); return Get(); }

  void Step(size_t num_steps=1) {
    while ( num_steps ) num_steps -= TryStep(num_steps);
  }

  const value_type& GetNext() { while (TryStep() == 0); return Get(); }

  using optional_ref_t = std::optional<std::reference_wrapper<
    const value_type
  >>;

  optional_ref_t GetNextOrNullopt() {
  return TryStep()
    ? optional_ref_t{ std::reference_wrapper{ Get() } }
    : std::nullopt;
  }

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

  decltype(auto) GetFractionReadsThatWereStale() const {
    return outlet.GetFractionReadsThatWereStale();
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

  auto GetDuctUID() const { return outlet.GetUID(); }

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

  void RegisterMeshID(const size_t mesh_id) const {
    outlet.RegisterMeshID(mesh_id);
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

  decltype(auto) LookupInletNodeID() const {
    return outlet.LookupInletNodeID();
  }

  decltype(auto) LookupOutletNodeID() const {
    return outlet.LookupOutletNodeID();
  }

  decltype(auto) LookupMeshID() const { return outlet.LookupMeshID(); }

};

} // namespace internal
} // namespace uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_OUTLET_CACHINGOUTLETWRAPPER_HPP_INCLUDE
