#pragma once

#include <iostream>
#include <memory>
#include <atomic>

#include "thread_utils.h"

#include "Duct.h"
#include "OccupancyCaps.h"
#include "OccupancyGuard.h"

template<typename T, size_t N=1024>
class Outlet {

#ifndef NDEBUG
  OccupancyCaps caps;
#endif

  using buffer_t = std::array<T, N>;

  std::shared_ptr<Duct<T,N>> duct;
  size_t read_position{N-1}; // TODO static assert N != 0
  size_t odometer{0};

  const pending_t & GetPending() const { return duct->GetPending(); }

  buffer_t & GetBuffer() { return duct->GetBuffer(); }

  const buffer_t & GetBuffer() const { return duct->GetBuffer(); }

  size_t Advance(const size_t step=1) {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("Advance", 1)};
#endif

    read_position = (read_position + step) % N;
    duct->Pop(step);
    ++odometer; // log number of *distinct* read events
    return step;
  }

  size_t FastForward() {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("FastForward", 1)};
#endif

    return Advance(GetPending());
  }

  const T& DoGet() {
    return GetBuffer().at(read_position);
  }

public:
  Outlet(
    std::shared_ptr<Duct<T,N>> duct_
  ) : duct(duct_) { ; }

  // non-blocking
  const T& GetCurrent() {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("GetCurrent", 1)};
#endif

    FastForward();
    return DoGet();
  }

  // blocking
  const T& GetNext() {
#ifndef NDEBUG
    const OccupancyGuard guard{caps.Get("GetNext", 1)};
#endif

    while (GetPending() == 0);
    Advance();
    return DoGet();
  }

  size_t ReadOdometer() const { return odometer; }

  std::string ToString() const {
    std::stringstream ss;
    ss << format_member("std::shared_ptr<Duct<T,N>> duct", *duct) << std::endl;
    ss << format_member(
      "GetBuffer().at(read_position - 1)",
      GetBuffer().at((read_position + N - 1) % N)
    ) << std::endl;
    ss << format_member(
      "GetBuffer().at(read_position)",
      GetBuffer().at(read_position)
    ) << std::endl;
    ss << format_member(
      "GetBuffer().at(read_position + 1)",
      GetBuffer().at((read_position + 1) % N)
    ) << std::endl;
    ss << format_member("size_t read_position", read_position) << std::endl;
    ss << format_member("size_t odometer", odometer);
    return ss.str();
  }

};
