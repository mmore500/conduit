#pragma once
#ifndef UIT_SPOUTS_INLET_HPP_INCLUDE
#define UIT_SPOUTS_INLET_HPP_INCLUDE

#include <atomic>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <stddef.h>
#include <utility>

#include "../../uitsl/debug/occupancy_audit.hpp"
#include "../../uitsl/nonce/CircularIndex.hpp"
#include "../../uitsl/utility/print_utils.hpp"

#include "../ducts/Duct.hpp"

namespace uit {

/**
 * Input to conduit transmission.
 *
 * Allows user to initiate
 *
 *  - potentially-blocking, assured transmisison via `Put`, or
 *  - non-blocking, potentially-dropped transmission via `TryPut`.
 *
 * An `Inlet` holds a `std::shared_ptr` to a `Duct` object, which manages data
 * transmission from the `Inlet`.
 *
 * An `Inlet`'s underlying `Duct` may be altered or replaced at runtime, for
 * example to provide thread-safe or process-safe transmission.
 *
 * - `EmplaceDuct` emplaces a new transmission implementation within
 *   the  existing `Duct` object. (See `include/ducts/Duct.hpp` for details.)
 * - `SplitDuct` makes a new `Duct` and points the `Inlet`'s `std::shared_ptr`
 *   to that `Duct`.
 *
 * If an `Outlet` holds a `std::shared_ptr` to the `Inlet`'s `Duct`, under an
 * `EmplaceDuct` call the `Duct`'s change in transmission implementation will
 * be visible to the `Outlet` and `Inlet` and the `Outlet` will still share a
 * `Duct`. However, under a `SplitDuct` call that `Outlet`'s `Duct` will be
 * unaffected. After a `SplitDuct` call, the `Inlet` and `Outlet` will hold
 * `std::shared_ptr`'s to separate `Duct`s.
 *
 * @tparam ImplSpec class with static and typedef members specifying
 *   implementation details for the conduit framework. See
 *   `include/config/ImplSpec.hpp`.
 *
 * @note End users should probably never have to directly instantiate this
 *   class. The `Conduit`, `Sink`, and `Source` classes take care of creating a
 *   `Duct` and tying it to an `Inlet` and/or `Outlet`. Better yet, the
 *   `MeshTopology` interface allows end users to construct a conduit network
 *    in terms of a connection topology and a mapping to assign nodes to
 *    threads and processes without having to manually construct `Conduits` and
 *    emplace necessary thread-safe and/or process-safe `Duct` implementations.
 */
template<typename ImplSpec_>
class Inlet {

public:
  using ImplSpec = ImplSpec_;

private:
  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ ImplSpec::N };

  using index_t = uitsl::CircularIndex<N>;

  /// TODO.
  using duct_t = internal::Duct<ImplSpec>;
  std::shared_ptr<duct_t> duct;

  /// How many blocking put operations have been performed?
  /// Instrumentation for communication profiling.
  size_t blocking_put_count{};

  /// How many nonblocking try put operations have been attempted?
  /// Instrumentation for communication profiling.
  size_t attempted_try_put_count{};

  /// How many times has Put blocked?
  /// Instrumentation for communication profiling.
  size_t puts_that_blocked_count{};

  // How many TryPut calls have dropped?
  /// Instrumentation for communication profiling.
  size_t dropped_put_count{};

  uitsl_occupancy_auditor;

  /**
   * TODO.
   *
   * @param val
   */
  bool DoTryPut(const T& val) { return duct->TryPut(val); }

  /**
   * TODO.
   *
   * @param val
   */
  template<typename P>
  bool DoTryPut(P&& val) { return duct->TryPut(std::forward<P>(val)); }

public:

  /**
   * TODO.
   *
   * @param duct_ TODO.
   */
  Inlet(
    std::shared_ptr<duct_t> duct_
  ) : duct(duct_) { ; }

  // potentially blocking
  /**
   * TODO.
   *
   * @param val TODO.
   */
  void Put(const T& val) {
    uitsl_occupancy_audit(1);

    ++blocking_put_count;
    bool was_blocked{ false };
    while (!DoTryPut(val)) was_blocked = true;

    puts_that_blocked_count += was_blocked;

  }

  // non-blocking
  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    uitsl_occupancy_audit(1);

    ++attempted_try_put_count;
    if ( DoTryPut(val) ) return true;
    else { ++dropped_put_count; return false; }

  }

  // non-blocking
  /**
   * TODO.
   *
   * @param val TODO.
   */
  template<typename P>
  bool TryPut(P&& val) {
    uitsl_occupancy_audit(1);

    ++attempted_try_put_count;
    if ( DoTryPut(std::forward<P>(val)) ) return true;
    else { ++dropped_put_count; return false; }

  }

  /**
   * TODO.
   *
   */
  bool TryFlush() { return duct->TryFlush(); }

  /**
   * TODO.
   *
   */
  void Flush() { while( !TryFlush() ); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumPutsAttempted() const {
    return attempted_try_put_count + blocking_put_count;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumTryPutsAttempted() const {
    return attempted_try_put_count;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumBlockingPuts() const {
    return blocking_put_count;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumTryPutsThatSucceeded() const {
    assert( attempted_try_put_count >= dropped_put_count );
    return attempted_try_put_count - dropped_put_count;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumPutsThatSucceededEventually() const {
    return blocking_put_count + GetNumTryPutsThatSucceeded();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumBlockingPutsThatSucceededImmediately() const {
    assert( blocking_put_count >= puts_that_blocked_count );
    return blocking_put_count - puts_that_blocked_count;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumPutsThatSucceededImmediately() const {
    return (
      GetNumTryPutsThatSucceeded()
      + GetNumBlockingPutsThatSucceededImmediately()
    );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumPutsThatBlocked() const { return puts_that_blocked_count; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumDroppedPuts() const { return dropped_put_count; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionTryPutsDropped() const {
    return dropped_put_count / static_cast<double>( attempted_try_put_count );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionTryPutsThatSucceeded() const {
    return 1.0 - GetFractionTryPutsDropped();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionBlockingPutsThatBlocked() const {
    return puts_that_blocked_count / static_cast<double>( blocking_put_count );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionBlockingPutsThatSucceededImmediately() const {
    return 1.0 - GetFractionBlockingPutsThatBlocked();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionPutsThatSucceededEventually() const {
    return (
      GetNumPutsThatSucceededEventually()
      / static_cast<double>( GetNumPutsAttempted() )
    );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionPutsThatSucceededImmediately() const {
    return (
      GetNumPutsThatSucceededImmediately()
      / static_cast<double>( GetNumPutsAttempted() )
    );
  }


  /**
   * TODO.
   *
   * @tparam WhichDuct
   * @tparam Args
   * @param args
   */
  template <typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    duct->template EmplaceImpl<WhichDuct>(std::forward<Args>(args)...);
  }

  /**
   * TODO.
   *
   * @tparam WhichDuct
   * @tparam Args
   * @param args
   */
  template <typename WhichDuct, typename... Args>
  void SplitDuct(Args&&... args) {
    duct = std::make_shared<duct_t>(
      std::in_place_type_t<WhichDuct>{},
      std::forward<Args>(args)...
    );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  typename duct_t::uid_t GetDuctUID() const { return duct->GetUID(); }

  std::optional<bool> HoldsIntraImpl() const { return duct->HoldsIntraImpl(); }

  std::optional<bool> HoldsThreadImpl() const {
    return duct->HoldsThreadImpl();
  }

  std::optional<bool> HoldsProcImpl() const { return duct->HoldsProcImpl(); }

  std::string WhichImplHeld() const { return duct->WhichImplHeld(); }

  void RegisterInletProc(const uitsl::proc_id_t proc) const {
    duct->RegisterInletProc(proc);
  }

  void RegisterInletThread(const uitsl::thread_id_t thread) const {
    duct->RegisterInletThread(thread);
  }

  void RegisterOutletProc(const uitsl::proc_id_t proc) const {
    duct->RegisterOutletProc(proc);
  }

  void RegisterOutletThread(const uitsl::thread_id_t thread) const {
    duct->RegisterOutletThread(thread);
  }

  void RegisterEdgeID(const size_t edge_id) const {
    duct->RegisterEdgeID(edge_id);
  }

  void RegisterInletNodeID(const size_t node_id) const {
    duct->RegisterInletNodeID(node_id);
  }

  void RegisterOutletNodeID(const size_t node_id) const {
    duct->RegisterOutletNodeID(node_id);
  }

  void RegisterMeshID(const size_t mesh_id) const {
    duct->RegisterMeshID(mesh_id);
  }

  std::optional<uitsl::proc_id_t> LookupOutletProc() const {
    return duct->LookupOutletProc();
  }

  std::optional<uitsl::thread_id_t> LookupOutletThread() const {
    return duct->LookupOutletThread();
  }

  std::optional<uitsl::proc_id_t> LookupInletProc() const {
    return duct->LookupInletProc();
  }

  std::optional<uitsl::thread_id_t> LookupInletThread() const {
    return duct->LookupInletThread();
  }

  std::optional<size_t> LookupEdgeID() const {
    return duct->LookupEdgeID();
  }

  std::optional<size_t> LookupInletNodeID() const {
    return duct->LookupInletNodeID();
  }

  std::optional<size_t> LookupOutletNodeID() const {
    return duct->LookupOutletNodeID();
  }

  std::optional<size_t> LookupMeshID() const { return duct->LookupMeshID(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string ToString() const {
    std::stringstream ss;
    ss << uitsl::format_member("duct_t duct", *duct) << '\n';
    ss << uitsl::format_member(
      "size_t attempted_try_put_count",
      attempted_try_put_count
    ) << '\n';
    ss << uitsl::format_member(
      "size_t blocking_put_count",
      blocking_put_count
    ) << '\n';
    ss << uitsl::format_member(
      "size_t dropped_put_count",
      dropped_put_count
    ) << '\n';
    ss << uitsl::format_member(
      "size_t puts_that_blocked_count",
      puts_that_blocked_count
    );
    return ss.str();
  }


};

} // namespace uit

#endif // #ifndef UIT_SPOUTS_INLET_HPP_INCLUDE
