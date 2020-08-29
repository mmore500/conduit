#pragma once

#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <stddef.h>
#include <utility>

#include "../parallel/thread_utils.hpp"
#include "../parallel/OccupancyCaps.hpp"
#include "../parallel/OccupancyGuard.hpp"
#include "../utility/CircularIndex.hpp"

#include "config.hpp"
#include "Duct.hpp"

namespace uit {

/**
 * Output from conduit transmission.
 *
 * Allows user to initiate
 *
 *  - potentially-blocking, strictly-sequential of the next unread received
 *     transmission, or
 *  - non-blocking fetch of the latest received transmission via `GetCurrent`.
 *
 * An `Outlet` holds a `std::shared_ptr` to a `Duct` object, which manages data
 * transmission to the `Outlet`.
 *
 * An `Outlet`'s underlying `Duct` may be altered or replaced at runtime, for
 * example to provide thread-safe or process-safe transmission.
 *
 * - `EmplaceDuct` emplaces a new transmission implementation within
 *   the  existing `Duct` object. (See `include/conduit/Duct.hpp` for details.)
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
 *   `include/conduit/ImplSpec.hpp`.
 *
 * @note End users should probably never have to directly instantiate this
 *   class. The `Conduit`, `Sink`, and `Source` classes take care of creating a
 *   `Duct` and tying it to an `Inlet` and/or `Outlet`. Better yet, the
 *   `MeshTopology` interface allows end users to construct a conduit network
 *    in terms of a connection topology and a mapping to assign nodes to
 *    threads and processes without having to manually construct `Conduits` and
 *    emplace necessary thread-safe and/or process-safe `Duct` implementations.
 */
template<typename ImplSpec>
class Outlet {

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

#ifndef NDEBUG
  OccupancyCaps caps;
#endif

  using index_t = CircularIndex<N>;

  using duct_t = internal::Duct<ImplSpec>;
  std::shared_ptr<duct_t> duct;

  // TODO move this to ImplSpec?
  static_assert(N > 0);

  /// How many times has outlet been read from?
  size_t read_count{0};

  /// How many times has current value changed?
  size_t revision_count{0};

  /// Total distance traversed through underlying buffer.
  size_t net_flux{0};

  /**
   * TODO.
   *
   * @return
   */
  size_t CountUnconsumedGets() { return duct->CountUnconsumedGets(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return duct->Get(); }

  /**
   * TODO.
   *
   * @param n TODO.
   */
  void ConsumeGets(const size_t n) {
  #ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("ConsumeGets", 1)};
  #endif

    Log( duct->ConsumeGets(n) );

  }

  /**
   * TODO.
   *
   * @param n TODO.
   */
  void Log(const size_t n) {
    ++read_count;
    revision_count += (n > 0);
    net_flux += n;
  }

public:

  /**
   * TODO.
   */
  Outlet(
    std::shared_ptr<duct_t> duct_
  ) : duct(duct_) { ; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& GetCurrent() {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("GetCurrent", 1)};
#endif

    ConsumeGets( std::numeric_limits<size_t>::max() );
    return Get();
  }

  /**
   * Get next received value.
   *
   * Blocking.
   *
   * @return TODO.
   */
  const T& GetNext() {
    #ifndef NDEBUG
      const OccupancyGuard guard{caps.Get("GetNext", 1)};
    #endif
    while (CountUnconsumedGets() == 0);
    ConsumeGets(1);
    return Get();
  }

  /**
   * Get next if available.
   *
   * Non-blocking.
   *
   * @return TODO.
   */
  std::optional<std::reference_wrapper<const T>> GetNextOrNullopt() {
    #ifndef NDEBUG
      const OccupancyGuard guard{caps.Get("GetNextOrNullopt", 1)};
    #endif
    if (CountUnconsumedGets()) {
      ConsumeGets(1);
      return std::optional{ std::reference_wrapper{ Get() } };
    } else return std::nullopt;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetReadCount() const { return read_count; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetRevisionCount() const { return revision_count; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetNetFlux() const { return net_flux; }

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
  typename duct_t::uid_t GetDuctUID() const {
    return duct->GetUID();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string ToString() const {
    std::stringstream ss;
    ss << format_member("std::shared_ptr<duct_t> duct", *duct) << std::endl;
    ss << format_member("size_t read_count", read_count) << std::endl;
    ss << format_member("size_t revision_count", revision_count) << std::endl;
    ss << format_member("size_t net_flux", net_flux);
    return ss.str();
  }

};

} // namespace uit
