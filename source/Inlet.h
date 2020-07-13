#pragma once

#include <iostream>
#include <utility>
#include <memory>
#include <atomic>

#include "config_utils.h"
#include "print_utils.h"

#include "CircularIndex.h"
#include "OccupancyCaps.h"
#include "OccupancyGuard.h"
#include "Duct.h"

template<typename T, size_t N=DEFAULT_BUFFER>
class Inlet {

#ifndef NDEBUG
  OccupancyCaps caps;
#endif

  using buffer_t = std::array<T, N>;
  using index_t = CircularIndex<N>;

  std::shared_ptr<Duct<T,N>> duct;
  index_t write_position{0};

  // number of times the inlet has been written to
  size_t successful_write_count{0};

  // number of times write attempts have blocked due to buffer space
  size_t blocked_write_count{0};

  // number of times write attempts have dropped due to buffer space
  size_t dropped_write_count{0};

  size_t GetAvailableCapacity() const { return duct->GetAvailableCapacity(); }

  T GetElement(const size_t n) const { return duct->GetElement(n); }

  void SetElement(const size_t n, const T& val) { duct->SetElement(n, val); }

  void Advance() {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("Advance", 1)};
#endif

    ++write_position;
    duct->Push();
    ++successful_write_count;
  }

  void DoPut(const T& val) {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("DoPut", 1)};
#endif

    SetElement(write_position, val);
    Advance();
  }

public:
  Inlet(
    std::shared_ptr<Duct<T,N>> duct_
  ) : duct(duct_) { ; }

  // potentially blocking
  void Put(const T& val) {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("Put", 1)};
#endif

    blocked_write_count += IsFull();
    while (IsFull());

    DoPut(val);

  }

  // non-blocking
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

  size_t GetSuccessfulWriteCount() const { return successful_write_count; }

  size_t GetBlockedWriteCount() const { return blocked_write_count; }

  size_t GetDroppedWriteCount() const { return dropped_write_count; }

  bool IsFull() const { return 0 == GetAvailableCapacity(); }

  template <typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    duct->template EmplaceDuct<WhichDuct>(std::forward<Args>(args)...);
  }

  template <typename WhichDuct, typename... Args>
  void SplitDuct(Args&&... args) {
    emp_assert(GetAvailableCapacity() == N);
    duct = std::make_shared<Duct<T,N>>();
    EmplaceDuct<WhichDuct>(args...);
  }

  std::string ToString() const {
    std::stringstream ss;
    ss << format_member("std::shared_ptr<Duct<T,N>> duct", *duct) << std::endl;
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
