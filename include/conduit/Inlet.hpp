#pragma once

#include <atomic>
#include <cstdint>
#include <iostream>
#include <memory>
#include <stddef.h>
#include <utility>

#include "../parallel/OccupancyCaps.hpp"
#include "../parallel/OccupancyGuard.hpp"
#include "../utility/CircularIndex.hpp"
#include "../utility/print_utils.hpp"

#include "config.hpp"
#include "Duct.hpp"

namespace uit {

/**
 * Input to conduit transmission.
 *
 * Allows user to initiate
 *
 *  - potentially-blocking, assured transmisison via `Put`, or
 *  - non-blocking, potentially-dropped transmission via `MaybePut`.
 *
 * An `Inlet` holds a `std::shared_ptr` to a `Duct` object, which manages data
 * transmission from the `Inlet`.
 *
 * An `Inlet`'s underlying `Duct` may be altered or replaced at runtime, for
 * example to provide thread-safe or process-safe transmission.
 *
 * - `EmplaceDuct` emplaces a new transmission implementation within
 *   the  existing `Duct` object. (See `include/conduit/Duct.hpp` for details.)
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
class Inlet {

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

#ifndef NDEBUG
  OccupancyCaps caps;
#endif

  using index_t = CircularIndex<N>;

  std::shared_ptr<internal::Duct<ImplSpec>> duct;
  // TODO this should be internal state to the duct
  index_t write_position{0};

  // number of times the inlet has been written to
  size_t successful_write_count{0};

  // number of times write attempts have blocked due to buffer space
  size_t blocked_write_count{0};

  // number of times write attempts have dropped due to buffer space
  size_t dropped_write_count{0};

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetAvailableCapacity() { return duct->GetAvailableCapacity(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T GetElement(const size_t n) const { return duct->GetElement(n); }

  /**
   * TODO.
   *
   * @param n
   * @param val
   * @return TODO.
   */
  void SetElement(const size_t n, const T& val) { duct->SetElement(n, val); }

  /**
   * TODO.
   */
  void Advance() {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("Advance", 1)};
#endif

    ++write_position;
    duct->Push();
    ++successful_write_count;
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  void DoPut(const T& val) {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("DoPut", 1)};
#endif

    SetElement(write_position, val);
    Advance();
  }

public:

  /**
   * TODO.
   *
   * @param duct_ TODO.
   */
  Inlet(
    std::shared_ptr<internal::Duct<ImplSpec>> duct_
  ) : duct(duct_) { ; }

  // potentially blocking
  /**
   * TODO.
   *
   * @param val TODO.
   */
  void Put(const T& val) {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("Put", 1)};
#endif

    blocked_write_count += IsFull();
    while (IsFull());

    DoPut(val);

  }

  // non-blocking
  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool MaybePut(const T& val) {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("MaybePut", 1)};
#endif

    if (IsFull()) {
      ++dropped_write_count;
      return false;
    }

    DoPut(val);

    return true;

  }

  /**
   * TODO.
   */
  void Prime() {
    __builtin_prefetch(duct->GetPosition(write_position+1), 0);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetSuccessfulWriteCount() const { return successful_write_count; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetBlockedWriteCount() const { return blocked_write_count; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t GetDroppedWriteCount() const { return dropped_write_count; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  bool IsFull() { return 0 == GetAvailableCapacity(); }

  /**
   * TODO.
   *
   * @tparam WhichDuct
   * @tparam Args
   * @param args
   */
  template <typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    emp_assert(GetAvailableCapacity() == N);
    duct->template EmplaceDuct<WhichDuct>(std::forward<Args>(args)...);
    duct->Initialize(write_position);
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
    emp_assert(GetAvailableCapacity() == N);
    duct = std::make_shared<internal::Duct<ImplSpec>>();
    EmplaceDuct<WhichDuct>(args...);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  typename Duct<ImplSpec>::uid_t GetDuctUID() const { return duct->GetUID(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  std::string ToString() const {
    std::stringstream ss;
    ss << format_member("std::shared_ptr<internal::Duct<ImplSpec>> duct", *duct) << std::endl;
    ss << format_member(
      "GetElement(write_position - 1)",
      GetElement(write_position - 1)
    ) << std::endl;
    ss << format_member(
      "GetElement(write_position)",
      GetElement(write_position)
    ) << std::endl;
    ss << format_member(
      "GetElement(write_position + 1)",
      GetElement(write_position + 1)
    ) << std::endl;

    ss << format_member("size_t write_position", write_position) << std::endl;
    ss << format_member(
      "size_t successful_write_count",
      successful_write_count
    ) << std::endl;
    ss << format_member(
      "size_t dropped_write_count",
      dropped_write_count
    ) << std::endl;;
    ss << format_member(
      "size_t blocked_write_count",
      blocked_write_count
    );
    return ss.str();
  }


};

} // namespace uit
