#pragma once

#include <iostream>
#include <memory>

#include "config_utils.h"
#include "thread_utils.h"

#include "CircularIndex.h"
#include "Duct.h"
#include "OccupancyCaps.h"
#include "OccupancyGuard.h"

template<typename T, size_t N=DEFAULT_BUFFER>
class Outlet {

#ifndef NDEBUG
  OccupancyCaps caps;
#endif

  using buffer_t = std::array<T, N>;
  using index_t = CircularIndex<N>;

  std::shared_ptr<Duct<T,N>> duct;

  static_assert(N > 0);
  index_t read_position{N-1};

  // number of times the outlet has been read from
  size_t read_count{0};

  // number of times the current value has changed
  size_t revision_count{0};

  // total distance traversed through underlying buffer
  size_t net_flux{0};

  size_t GetPending() const { return duct->GetPending(); }

  T GetElement(const size_t n) const { return duct->GetElement(n); }

  size_t Advance(const size_t step=1) {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("Advance", 1)};
#endif

    read_position += step;
    duct->Pop(step);

    Log(step);

    return step;
  }

  void Log(const size_t step) {
    ++read_count;
    revision_count += (step > 0);
    net_flux += step;
  }

  size_t FastForward() {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("FastForward", 1)};
#endif

    return Advance(GetPending());
  }

  T DoGet() { return GetElement(read_position); }

public:
  Outlet(
    std::shared_ptr<Duct<T,N>> duct_
  ) : duct(duct_) { ; }

  // non-blocking
  // TODO rename GetLatest
  T GetCurrent() {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("GetCurrent", 1)};
#endif

    FastForward();
    return DoGet();
  }

  // blocking
  T GetNext() {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("GetNext", 1)};
#endif

    while (GetPending() == 0);
    Advance();
    return DoGet();
  }

  void Prime() const {
    __builtin_prefetch(duct->GetPosition(read_position+1), 0);
  }

  size_t GetReadCount() const { return read_count; }

  size_t GetRevisionCount() const { return revision_count; }

  size_t GetNetFlux() const { return net_flux; }

  template <typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    FastForward();
    emp_assert(GetPending() == 0);
    duct->template EmplaceDuct<WhichDuct>(std::forward<Args>(args)...);
    duct->Initialize(read_position+1);
  }

  template <typename WhichDuct, typename... Args>
  void SplitDuct(Args&&... args) {
    emp_assert(GetPending() == 0);
    duct = std::make_shared<Duct<T, N>>();
    EmplaceDuct<WhichDuct>(args...);
  }

  std::string ToString() const {
    std::stringstream ss;
    ss << format_member("std::shared_ptr<Duct<T,N>> duct", *duct) << std::endl;
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
