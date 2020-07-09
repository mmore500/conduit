#pragma once

#include <iostream>
#include <memory>
#include <atomic>

#include "Duct.h"

template<typename T, size_t N=1024>
class Outlet {

  using buffer_t = std::array<T, N>;

  std::shared_ptr<Duct<T,N>> duct;
  size_t read_position{N-1}; // TODO static assert N != 0
  size_t odometer{0};

  const pending_t & GetPending() const { return duct->GetPending(); }

  buffer_t & GetBuffer() { return duct->GetBuffer(); }

  const buffer_t & GetBuffer() const { return duct->GetBuffer(); }

  size_t Advance(const size_t step=1) {
    read_position = (read_position + step) % N;
    duct->Pop(step);
    ++odometer; // log number of *distinct* read events
    return step;
  }

  size_t FastForward() {
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
    FastForward();
    return DoGet();
  }

  // blocking
  const T& GetNext() {
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
