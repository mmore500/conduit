#pragma once

#include <algorithm>
#include <array>
#include <limits>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../third-party/Empirical/source/base/vector.h"
#include "../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../mpi/mpi_utils.hpp"
#include "../../../../distributed/RdmaPacket.hpp"
#include "../../../../distributed/RdmaWindowManager.hpp"
#include "../../../../utility/CircularIndex.hpp"
#include "../../../../utility/identity.hpp"
#include "../../../../utility/print_utils.hpp"
#include "../../../../utility/WarnOnce.hpp"

#include "../../../config.hpp"
#include "../../../InterProcAddress.hpp"

#include "../../backend/RdmaBackEnd.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class WindowDuct {

public:

  using BackEndImpl = uit::RdmaBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  using packet_t = uit::RdmaPacket<T>;

  packet_t cache{};
  size_t cur_epoch{};

  const uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;
  const int byte_offset;

  size_t CountUnconsumedGets() const { return 1; }

public:

  WindowDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_
  ) : address(address_)
  , back_end(back_end_)
  , byte_offset(
    address.GetOutletProc() == uit::get_rank(address.GetComm())
      ? back_end->GetWindowManager().Acquire(
        address.GetInletProc(),
        emp::vector<std::byte>(
          reinterpret_cast<std::byte*>(&cache),
          reinterpret_cast<std::byte*>(&cache) + sizeof(packet_t)
        )
      ) : -1
  ) {
    if (address.GetOutletProc() == uit::get_rank(address.GetComm())) {
      MPI_Request req;
      UIT_Isend(
        &byte_offset, // const void *buf
        1, // int count
        MPI_INT, // MPI_Datatype datatype
        address.GetInletProc(), // int dest
        address.GetTag(), // int tag
        address.GetComm(), // MPI_Comm comm
        &req // MPI_Request * request
      );
      UIT_Request_free(&req); //TODO test for completion in destructor?
    }

  }

  [[noreturn]] bool TryPut(const T&) { throw "TryPut called on WindowDuct"; }

  [[noreturn]] bool IsReadyForPut() {
    throw "IsReadyForPut called on WindowDuct";
  }

  size_t TryConsumeGets(const size_t requested) {
    emp_assert( requested == std::numeric_limits<size_t>::max() );

    // lock own window
    back_end->GetWindowManager().LockShared( address.GetInletProc() );
    std::memcpy(
      reinterpret_cast<std::byte*>(&cache),
      back_end->GetWindowManager().GetBytes(
        address.GetInletProc(),
        byte_offset
      ),
      sizeof(packet_t)
    );
    back_end->GetWindowManager().Unlock( address.GetInletProc() );

    emp_assert( cache.GetEpoch() >= cur_epoch , cache.GetEpoch(), cur_epoch );

    const size_t elapsed_epochs = cache.GetEpoch() - cur_epoch;

    cur_epoch = cache.GetEpoch();

    return elapsed_epochs;
  }

  const T& Get() const { return cache.GetData(); }

  T& Get() { return cache.GetData(); }

  static std::string GetName() { return "WindowDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << std::endl;
    ss << format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << format_member("int get_rank()", get_rank()) << std::endl;
    return ss.str();
  }

};

} // namespace uit
