#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_TEMPLATED_ACCUMULATINGPOOLEDINLETDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_TEMPLATED_ACCUMULATINGPOOLEDINLETDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <memory>
#include <stddef.h>
#include <string>

#include <mpi.h>

#include "../../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../../third-party/Empirical/source/base/optional.h"
#include "../../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../../uitsl/utility/print_utils.hpp"

#include "../../../../../setup/InterProcAddress.hpp"

#include "../../backend/AccumulatingPooledBackEnd.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<
  template<typename> typename BackingDuct,
  typename ImplSpec
>
class AccumulatingPooledInletDuct {

public:

  using BackEndImpl = uit::AccumulatingPooledBackEnd<ImplSpec, BackingDuct>;

private:

  using T = typename ImplSpec::T;

  uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  using pool_t = typename BackEndImpl::inlet_pool_t;
  emp::optional<std::reference_wrapper<pool_t>> pool;
  size_t pool_index;

  void SetupPool() {
    pool = back_end->GetInletPool(address);
    pool_index = pool->get().Lookup(address);
  }

public:

  AccumulatingPooledInletDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_
  ) : address(address_)
  , back_end(back_end_)
  { back_end->RegisterInletSlot(address); }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    if (!pool.has_value()) SetupPool();
    return pool->get().TryPut(val, pool_index);
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() {
    if (!pool.has_value()) SetupPool();
    return pool->get().TryFlush(pool_index);
  }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    throw "ConsumeGets called on AccumulatingPooledInletDuct";
  }

  [[noreturn]] const T& Get() const {
    throw "Get called on AccumulatingPooledInletDuct";
  }

  [[noreturn]] T& Get() {
    throw "Get called on AccumulatingPooledInletDuct";
  }

  static std::string GetType() { return "AccumulatingPooledInletDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << std::endl;
    return ss.str();
  }

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_TEMPLATED_ACCUMULATINGPOOLEDINLETDUCT_HPP_INCLUDE
