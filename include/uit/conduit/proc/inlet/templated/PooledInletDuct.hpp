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
template<
  template<typename> typename BackingDuct,
  typename ImplSpec
>
class PooledInletDuct {

public:

  using BackEndImpl = uit::PooledBackEnd<ImplSpec, BackingDuct>;

private:

  using T = typename ImplSpec::T;

  uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  using pool_t = typename BackEndImpl::inlet_pool_t;
  std::optional<std::reference_wrapper<pool_t>> pool;
  size_t pool_index;

  void SetupPool() {
    pool = back_end->GetInletPool(address);
    pool_index = pool->get().Lookup(address);
  }

public:

  PooledInletDuct(
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
  bool TryFlush() { return back_end->get().TryFlush(); }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    throw "ConsumeGets called on PooledInletDuct";
  }

  [[noreturn]] const T& Get() const {
    throw "Get called on PooledInletDuct";
  }

  [[noreturn]] T& Get() {
    throw "Get called on PooledInletDuct";
  }

  static std::string GetType() { return "PooledInletDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    return ss.str();
  }

};

} // namespace uit
