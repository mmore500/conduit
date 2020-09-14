#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../mpi/mpi_utils.hpp"
#include "../../../../mpi/Request.hpp"
#include "../../../../utility/CircularIndex.hpp"
#include "../../../../utility/identity.hpp"
#include "../../../../utility/print_utils.hpp"

#include "../../../InterProcAddress.hpp"

#include "../../backend/PooledBackEnd.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<template<typename> typename BackingDuct, typename ImplSpec>
class PooledOutletDuct {

public:

  using BackEndImpl = uit::PooledBackEnd<ImplSpec, BackingDuct>;

private:

  using T = typename ImplSpec::T;

  uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  using pool_t = typename BackEndImpl::outlet_pool_t;
  std::optional<std::reference_wrapper<pool_t>> pool;
  size_t pool_index;

  void SetupPool() {
    pool = back_end->GetOutletPool(address);
    pool_index = pool->get().Lookup(address);
  }

public:

  PooledOutletDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_
  ) : address(address_)
  , back_end(back_end_)
  { back_end->RegisterOutletSlot(address); }

  [[noreturn]] bool TryPut(const T&) const {
    throw "TryPut called on PooledOutletDuct";
  }

  [[noreturn]] bool TryFlush() const {
    throw "Flush called on PooledOutletDuct";
  }

  /**
   * TODO.
   *
   * @param num_requested TODO.
   * @return number items consumed.
   */
  size_t TryConsumeGets(const size_t num_requested) {

    if (!pool.has_value()) SetupPool();
    return pool->get().TryConsumeGets(num_requested, address);

  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const {
    if (!pool.has_value()) SetupPool();
    return pool->get().Get(pool_index);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() {
    if (!pool.has_value()) SetupPool();
    return pool->get().Get(pool_index);
  }

  static std::string GetName() { return "PooledOutletDuct"; }

  static constexpr bool CanStep() {
    return BackingDuct<ImplSpec>::OutletImpl::CanStep();
  }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    return ss.str();
  }

};

} // namespace uit
