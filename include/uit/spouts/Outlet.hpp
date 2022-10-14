#pragma once
#ifndef UIT_SPOUTS_OUTLET_HPP_INCLUDE
#define UIT_SPOUTS_OUTLET_HPP_INCLUDE

#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <stddef.h>
#include <utility>

#include "../../uitsl/debug/occupancy_audit.hpp"
#include "../../uitsl/nonce/CircularIndex.hpp"
#include "../../uitsl/parallel/thread_utils.hpp"

#include "../ducts/Duct.hpp"

namespace uit {

/**
 * Output from conduit transmission.
 *
 * Allows user to initiate
 *
 *  - potentially-blocking, strictly-sequential of the next unread received
 *     transmission, or
 *  - non-blocking fetch of the latest received transmission via `JumpGet`.
 *
 * An `Outlet` holds a `std::shared_ptr` to a `Duct` object, which manages data
 * transmission to the `Outlet`.
 *
 * An `Outlet`'s underlying `Duct` may be altered or replaced at runtime, for
 * example to provide thread-safe or process-safe transmission.
 *
 * - `EmplaceDuct` emplaces a new transmission implementation within
 *   the  existing `Duct` object. (See `include/ducts/Duct.hpp` for details.)
 * - `SplitDuct` makes a new `Duct` and points the `Outlet`'s `std::shared_ptr`
 *   to that `Duct`.
 *
 * If an `Inlet` holds a `std::shared_ptr` to the `Outlet`'s `Duct`, under an
 * `EmplaceDuct` call the `Duct`'s change in transmission implementation will
 * be visible to the `Inlet` and the `Inlet` and `Outlet` will still share a
 * `Duct`. However, under a `SplitDuct` call that `Inlet`'s `Duct` will be
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
class Outlet {

public:
  using ImplSpec = ImplSpec_;

private:
  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using index_t = uitsl::CircularIndex<N>;

  using duct_t = internal::Duct<ImplSpec>;
  std::shared_ptr<duct_t> duct;

  // TODO move this to ImplSpec?
  static_assert(N > 0);

  /// How many times has outlet been read from?
  /// Instrumentation for communication profiling.
  mutable size_t read_count{0};

  /// Has the current revision been viewed?
  /// Instrumentation for communication profiling.
  mutable bool cur_revision_unread{true};

  /// How many unique revisions have been read?
  /// Instrumentation for communication profiling.
  mutable size_t fresh_read_count{0};

  /// How many times has current value changed (i.e., a pull succeeded)?
  /// Instrumentation for communication profiling.
  /// Start at 1 to account for default-constructed initial value.
  size_t revision_count{1};

  /// Total distance traversed through underlying buffer.
  /// Instrumentation for communication profiling.
  size_t net_flux{0};

  /// Number of times try_step or jump was called.
  /// Instrumentation for communication profiling.
  size_t nonblocking_pull_attempt_count{0};

  /// Number of times nonblocking pull retrieved a fresh value.
  /// Instrumentation for communication profiling.
  size_t laden_nonblocking_pull_count{0};

  /// Number of times step was called.
  /// Instrumentation for communication profiling.
  size_t blocking_pull_count{0};

  /// Number of times step blocked.
  /// Instrumentation for communication profiling.
  size_t pulls_that_blocked_count{0};

  uitsl_occupancy_auditor;

  /**
   * TODO.
   *
   * @param n TODO.
   */
  size_t TryConsumeGets(const size_t n) {
    uitsl_occupancy_audit(1);
    return LogStep( duct->TryConsumeGets(n) );
  }

  /**
   * TODO.
   *
   * @param n TODO.
   */
  size_t LogStep(const size_t n) {
    revision_count += (n > 0);
    if (n > 0) cur_revision_unread = true;
    net_flux += n;
    return n;
  }

  void LogRead() const {
    ++read_count;
    fresh_read_count += cur_revision_unread;
    cur_revision_unread = false;
  }

public:

  /**
   * TODO.
   */
  Outlet(
    std::shared_ptr<duct_t> duct_
  ) : duct(duct_) { ; }

  size_t TryStep(const size_t num_steps=1) {
    ++nonblocking_pull_attempt_count;
    const size_t res = TryConsumeGets(num_steps);
    if ( res ) ++laden_nonblocking_pull_count;
    return res;
  }

  size_t Jump() {
    return TryStep( std::numeric_limits<size_t>::max() );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { LogRead(); return std::as_const(*duct).Get(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { LogRead(); return duct->Get(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& JumpGet() {
    uitsl_occupancy_audit(1);
    Jump();
    return Get();
  }

  /**
   * Step forward num_steps values.
   *
   * Blocking.
   *
   * @return TODO.
   */
  void Step(size_t num_steps=1) {
    uitsl_occupancy_audit(1);
    blocking_pull_count += num_steps;

    while (num_steps) {
      size_t uncounted_steps{};
      bool was_blocked{ false };
      do {
        uncounted_steps = TryConsumeGets(num_steps);
        was_blocked |= (uncounted_steps == 0);
      } while ( uncounted_steps == 0 );

      num_steps -= uncounted_steps;
      pulls_that_blocked_count += was_blocked;
    }

  }

  /**
   * Get next received value.
   *
   * Blocking.
   *
   * @return TODO.
   */
  const T& GetNext(const size_t num_steps=1) {
    Step(num_steps);
    return Get();
  }

  using optional_ref_t = std::optional<std::reference_wrapper<const T>>;

  /**
   * Get next if available.
   *
   * Non-blocking.
   *
   * @return TODO.
   */
  optional_ref_t GetNextOrNullopt() {
    uitsl_occupancy_audit(1);
    return TryStep()
      ? optional_ref_t{ std::reference_wrapper{ Get() } }
      : std::nullopt;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumReadsPerformed() const { return read_count; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumReadsThatWereFresh() const { return fresh_read_count; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumReadsThatWereStale() const {
    assert( read_count >= fresh_read_count );
    return read_count - fresh_read_count;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumRevisionsPulled() const { return revision_count; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumTryPullsAttempted() const {
    return nonblocking_pull_attempt_count;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumBlockingPulls() const {
    return blocking_pull_count;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumBlockingPullsThatBlocked() const {
    return pulls_that_blocked_count;
  }

  /**
   * TODO.
   *
   * @node identical to GetNumTryPullsThatWereLaden
   * @return TODO.
   */
  size_t GetNumRevisionsFromTryPulls() const {
    return laden_nonblocking_pull_count;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumRevisionsFromBlockingPulls() const {
    assert(revision_count >= GetNumRevisionsFromTryPulls());
    return revision_count - GetNumRevisionsFromTryPulls();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumPullsAttempted() const {
    return nonblocking_pull_attempt_count + blocking_pull_count;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumPullsThatWereLadenEventually() const {
    return GetNumTryPullsThatWereLaden() + GetNumBlockingPulls();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumBlockingPullsThatWereLadenImmediately() const {
    assert( GetNumBlockingPulls() >= GetNumBlockingPullsThatBlocked() );
    return GetNumBlockingPulls() - GetNumBlockingPullsThatBlocked();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumBlockingPullsThatWereLadenEventually() const {
    return GetNumBlockingPulls();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumPullsThatWereLadenImmediately() const {
    return GetNumTryPullsThatWereLaden()
    + GetNumBlockingPullsThatWereLadenImmediately();
  }

  /**
   * TODO.
   *
   * @note Identical to `GetNumRevisionsFromTryPulls`.
   * @return TODO.
   */

  size_t GetNumTryPullsThatWereLaden() const {
    return laden_nonblocking_pull_count;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNumTryPullsThatWereUnladen() const {
    assert(nonblocking_pull_attempt_count >= laden_nonblocking_pull_count);
    return nonblocking_pull_attempt_count - laden_nonblocking_pull_count;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionTryPullsThatWereLaden() const {
    assert(GetNumTryPullsThatWereLaden() >= GetNumTryPullsAttempted());
    return (
      GetNumTryPullsThatWereLaden()
      / static_cast<double>( GetNumTryPullsAttempted() )
    );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionTryPullsThatWereUnladen() const {
    return 1.0 - GetFractionTryPullsThatWereLaden();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionBlockingPullsThatBlocked() const {
    assert(
      GetNumBlockingPullsThatBlocked()
      >= GetNumBlockingPulls()
    );
    return (
      GetNumBlockingPullsThatBlocked()
      / static_cast<double>( GetNumBlockingPulls() )
    );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionBlockingPullsThatWereLadenImmediately() const {
    return 1.0 - GetFractionBlockingPullsThatBlocked();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionPullsThatWereLadenImmediately() const {
    assert(GetNumPullsThatWereLadenImmediately() >= GetNumPullsAttempted());
    return (
      GetNumPullsThatWereLadenImmediately()
      / static_cast<double>( GetNumPullsAttempted() )
    );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionPullsThatWereLadenEventually() const {
    assert(GetNumPullsThatWereLadenEventually() >= GetNumPullsAttempted());
    return (
      GetNumPullsThatWereLadenEventually()
      / static_cast<double>( GetNumPullsAttempted() )
    );
  }


  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNetFluxThroughDuct() const { return net_flux; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionReadsThatWereFresh() const {
    return fresh_read_count / static_cast<double>(read_count);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionReadsThatWereStale() const {
    return 1.0 - GetFractionReadsThatWereFresh();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionRevisionsThatWereRead() const {
    return fresh_read_count / static_cast<double>(revision_count);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionRevisionsThatWereNotRead() const {
    return 1.0 - GetFractionRevisionsThatWereRead();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionDuctFluxThatWasSteppedThrough() const {
    return revision_count / static_cast<double>(net_flux);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionDuctFluxThatWasJumpedOver() const {
    return 1.0 - GetFractionDuctFluxThatWasSteppedThrough();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  double GetFractionDuctFluxThatWasRead() const {
    return fresh_read_count / static_cast<double>(net_flux);
  }

  /**
   * TODO.
   *
   * @tparam WhichDuct TODO
   * @tparam Args TODO
   * @param args TODO
   */
  template <typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    duct->template EmplaceImpl<WhichDuct>(std::forward<Args>(args)...);
  }

  /**
   * TODO.
   *
   * @tparam WhichDuct TODO
   * @tparam Args TODO
   * @param args TODO
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

  bool CanStep() const { return duct->CanStep(); }

  // exclusively for instrumentation purposes
  void RegisterInletProc(const uitsl::proc_id_t proc) const {
    duct->RegisterInletProc(proc);
  }

  // exclusively for instrumentation purposes
  void RegisterInletThread(const uitsl::thread_id_t thread) const {
    duct->RegisterInletThread(thread);
  }

  // exclusively for instrumentation purposes
  void RegisterOutletProc(const uitsl::proc_id_t proc) const {
    duct->RegisterOutletProc(proc);
  }

  // exclusively for instrumentation purposes
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
    ss << uitsl::format_member("std::shared_ptr<duct_t> duct", *duct) << '\n';
    ss << uitsl::format_member("size_t read_count", read_count) << '\n';
    ss << uitsl::format_member("size_t revision_count", revision_count) << '\n';
    ss << uitsl::format_member("size_t net_flux", net_flux);
    return ss.str();
  }

};

} // namespace uit

#endif // #ifndef UIT_SPOUTS_OUTLET_HPP_INCLUDE
