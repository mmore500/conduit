#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <stddef.h>

#include "../parallel/thread_utils.hpp"
#include "../parallel/OccupancyCaps.hpp"
#include "../parallel/OccupancyGuard.hpp"
#include "../utility/CircularIndex.hpp"

#include "config.hpp"
#include "Duct.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class Outlet {

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

#ifndef NDEBUG
  OccupancyCaps caps;
#endif

  using index_t = CircularIndex<N>;

  std::shared_ptr<Duct<ImplSpec>> duct;

  static_assert(N > 0);
  // TODO this should be internal state to the duct
  index_t read_position{N-1};

  // number of times the outlet has been read from
  size_t read_count{0};

  // number of times the current value has changed
  size_t revision_count{0};

  // total distance traversed through underlying buffer
  size_t net_flux{0};

  /**
   * TODO.
   *
   * @return
   */
  size_t GetPending() { return duct->GetPending(); }

  /**
   * TODO.
   *
   * @param n TODO.
   *
   * @return TODO.
   */
  T GetElement(const size_t n) const { return duct->GetElement(n); }

  /**
   * TODO.
   *
   * @param step TODO.
   *
   * @return TODO.
   */
  size_t Advance(const size_t step=1) {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("Advance", 1)};
#endif

    read_position += step;
    duct->Pop(step);

    Log(step);

    return step;
  }

  /**
   * TODO.
   *
   * @param step TODO.
   */
  void Log(const size_t step) {
    ++read_count;
    revision_count += (step > 0);
    net_flux += step;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t FastForward() {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("FastForward", 1)};
#endif

    return Advance(GetPending());
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T DoGet() { return GetElement(read_position); }

public:

  /**
   * TODO.
   */
  Outlet(
    std::shared_ptr<Duct<ImplSpec>> duct_
  ) : duct(duct_) { ; }

  // non-blocking
  // TODO rename GetLatest
  /**
   * TODO.
   *
   * @return TODO.
   */
  T GetCurrent() {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("GetCurrent", 1)};
#endif

    FastForward();
    return DoGet();
  }

  // blocking
  /**
   * TODO.
   *
   * @return TODO.
   */
  T GetNext() {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("GetNext", 1)};
#endif

    while (GetPending() == 0);
    Advance();
    return DoGet();
  }

  /**
   * TODO.
   */
  void Prime() const {
    __builtin_prefetch(duct->GetPosition(read_position+1), 0);
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
   * @return TODO.
   * @return TODO.
   */
  template <typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    FastForward();
    emp_assert(GetPending() == 0);
    duct->template EmplaceDuct<WhichDuct>(std::forward<Args>(args)...);
    duct->Initialize(read_position+1);
  }

  /**
   * TODO.
   *
   * @tparam WhichDuct TODO
   * @tparam Args TODO
   * @param args TODO
   * @return TODO.
   */
  template <typename WhichDuct, typename... Args>
  void SplitDuct(Args&&... args) {
    emp_assert(GetPending() == 0);
    duct = std::make_shared<Duct<ImplSpec>>();
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
    ss << format_member("std::shared_ptr<Duct<ImplSpec>> duct", *duct) << std::endl;
    ss << format_member(
      "GetElement(read_position - 1)",
      GetElement(read_position - 1)
    ) << std::endl;
    ss << format_member(
      "GetElement(read_position)",
      GetElement(read_position)
    ) << std::endl;
    ss << format_member(
      "GetElement(read_position + 1)",
      GetElement(read_position + 1)
    ) << std::endl;
    ss << format_member("size_t read_position", read_position) << std::endl;
    ss << format_member("size_t read_count", read_count) << std::endl;
    ss << format_member("size_t revision_count", revision_count) << std::endl;
    ss << format_member("size_t net_flux", net_flux);
    return ss.str();
  }

};

} // namespace uit
