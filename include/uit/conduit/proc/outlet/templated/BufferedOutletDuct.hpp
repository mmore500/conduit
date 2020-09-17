#pragma once

#include <algorithm>
#include <deque>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../uitsl/nonce/ScopeGuard.hpp"
#include "../../../../../uitsl/utility/print_utils.hpp"

#include "../../../InterProcAddress.hpp"

#include "../../inlet/templated/impl/BufferSpec.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<template<typename> typename BackingDuct, typename ImplSpec>
class BufferedOutletDuct {

  using BufferSpec = uit::internal::BufferSpec<ImplSpec, BackingDuct>;

public:

  using BackEndImpl = typename BackingDuct<BufferSpec>::BackEndImpl;

private:

  using T = typename ImplSpec::T;
  constexpr static inline size_t B{ ImplSpec::B };

  using BackingOutlet = typename BackingDuct<BufferSpec>::OutletImpl;
  BackingOutlet outlet;

  const uitsl::ScopeGuard guard{ [this](){ outlet.Get().emplace_back(); } };
  using current_t = typename BufferSpec::T::iterator;
  current_t current{ std::begin(outlet.Get()) };

  size_t JumpConsume() {
    const size_t num_buffers_consumed = outlet.TryConsumeGets(
      std::numeric_limits<size_t>::max()
    );
    emp_assert( !outlet.Get().empty() );
    current = std::prev( std::end( outlet.Get() ) );
    // estimate number of individual items consumed
    return num_buffers_consumed * outlet.Get().size();
  }

  size_t StepConsume(const size_t num_requested) {
    size_t num_requested_countdown{ num_requested };

    do {

      emp_assert( !outlet.Get().empty() );
      emp_assert(std::distance(std::next(current),std::end(outlet.Get())) >= 0);

      const size_t cur_step = std::min(
        static_cast<size_t>(
          std::distance( std::next(current), std::end(outlet.Get()) )
        ),
        num_requested_countdown
      );
      current += cur_step;
      num_requested_countdown -= cur_step;

    } while(
      num_requested_countdown
      && outlet.TryConsumeGets( 1 )
      && [this, &num_requested_countdown](){
        current = std::begin(outlet.Get());
        --num_requested_countdown;
        return true;
      }()
      && num_requested_countdown
    );

    return num_requested - num_requested_countdown;
  }

public:

  BufferedOutletDuct(
    const uit::InterProcAddress& address,
    std::shared_ptr<BackEndImpl> back_end
  ) : outlet(address, back_end)
  { ; }

  [[noreturn]] bool TryPut(const T&) const {
    throw "TryPut called on BufferedOutletDuct";
  }

  [[noreturn]] bool TryFlush() const {
    throw "Flush called on BufferedOutletDuct";
  }

  /**
   * TODO.
   *
   * @param num_requested TODO.
   * @return number items consumed.
   */
  size_t TryConsumeGets(const size_t num_requested) {

    if ( num_requested == std::numeric_limits<size_t>::max() ) {
      return JumpConsume();
    } else return StepConsume(num_requested);

  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return *current; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { return *current; }

  static std::string GetName() { return "BufferedOutletDuct"; }

  static constexpr bool CanStep() { return BackingOutlet::CanStep(); }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << std::endl;
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << std::endl;
    return ss.str();
  }

};

} // namespace uit
