#pragma once

#include <iostream>
#include <utility>
#include <memory>
#include <atomic>

#include "print_utils.h"

#include "OccupancyCaps.h"
#include "OccupancyGuard.h"
#include "Duct.h"

template<typename T, size_t N=1024>
class Inlet {

#ifndef NDEBUG
  OccupancyCaps caps;
#endif

  using buffer_t = std::array<T, N>;

  std::shared_ptr<Duct<T,N>> duct;
  size_t write_position{0};

  // number of times the inlet has been written to
  size_t write_count{0};

  // number of times write attempts have blocked due to buffer space
  size_t blocked_write_count{0};

  // number of times write attempts have dropped due to buffer space
  size_t dropped_write_count{0};


  const pending_t & GetPending() const { return duct->GetPending(); }

  buffer_t & GetBuffer() { return duct->GetBuffer(); }

  const buffer_t & GetBuffer() const { return duct->GetBuffer(); }

  void Advance() {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("Advance", 1)};
#endif

    write_position = (write_position + 1) % N;
    duct->Push();
    ++write_count;
  }

  void DoPut(const T& val) {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("DoPut", 1)};
#endif

    GetBuffer().at(write_position) = val;
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

  size_t GetWriteCount() const { return write_count; }

  size_t GetBlockedWriteCount() const { return blocked_write_count; }

  size_t GetDroppedWriteCount() const { return dropped_write_count; }

  bool IsFull() const { return GetPending() == N - 1; }

  std::string ToString() const {
    std::stringstream ss;
    ss << format_member("std::shared_ptr<Duct<T,N>> duct", *duct) << std::endl;
    ss << format_member(
      "GetBuffer().at(write_position - 1)",
      GetBuffer().at((write_position + N - 1) % N)
    ) << std::endl;
    ss << format_member(
      "GetBuffer().at(write_position)",
      GetBuffer().at(write_position)
    ) << std::endl;
    ss << format_member(
      "GetBuffer().at(write_position + 1)",
      GetBuffer().at((write_position + 1) % N)
    ) << std::endl;

    ss << format_member("size_t write_position", write_position) << std::endl;
    ss << format_member("size_t write_count", write_count);
    ss << format_member("size_t dropped_write_count", dropped_write_count);
    ss << format_member("size_t blocked_write_count", blocked_write_count);
    return ss.str();
  }


};
