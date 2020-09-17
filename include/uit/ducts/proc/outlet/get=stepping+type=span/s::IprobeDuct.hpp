#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <optional>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../third-party/Empirical/source/base/vector.h"
#include "../../../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../uitsl/mpi/Request.hpp"
#include "../../../../../uitsl/nonce/CircularIndex.hpp"
#include "../../../../../uitsl/utility/imemstream.hpp"
#include "../../../../../uitsl/utility/print_utils.hpp"
#include "../../../../../uitsl/initialization/Uninitialized.hpp"

#include "../../../../config/InterProcAddress.hpp"

#include "../../backend/RuntimeSizeBackEnd.hpp"

namespace uit {
namespace s {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class IprobeDuct {

public:

  using BackEndImpl = uit::RuntimeSizeBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  constexpr inline static size_t N{ImplSpec::N};

  size_t pending_gets{};

  const uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  // most vexing parse
  T buffer = T(
    back_end->HasSize()
    ? back_end->GetSize()
    : 0
  );


  void PerformReceive(const MPI_Status& status) {
    const int msg_len = uitsl::get_count(status, MPI_BYTE);

    emp_assert(msg_len % sizeof(typename T::value_type) == 0);
    buffer.resize(msg_len / sizeof(typename T::value_type));

    emp_assert( !back_end->HasSize() || back_end->GetSize() == buffer.size() );

    UITSL_Recv(
      buffer.data(), // void* buf: initial address of receive buffer
      msg_len, // int count: maximum number of elements in receive buffer
      MPI_BYTE,// MPI_Datatype datatype
      // datatype of each receive buffer element
      address.GetInletProc(), // int source: rank of source
      address.GetTag(), // int tag: message tag
      address.GetComm(), // MPI_Comm comm: communicator
      MPI_STATUS_IGNORE // MPI_Status *status: status object
    );

  }

  bool TryReceive() {

    MPI_Status status;
    int flag{};
    UITSL_Iprobe(
      address.GetInletProc(), // int source
      // source rank, or MPI_ANY_SOURCE (integer)
      address.GetTag(), // int tag: tag value or MPI_ANY_TAG (integer)
      address.GetComm(), // MPI_Comm comm: communicator (handle)
      &flag, // int *flag
      // True if a message with the specified source, tag, and communicator
      // is available (logical)
      &status // MPI_Status *status: status object
    );

    if (flag) PerformReceive(status);

    return flag;

  }

  // returns number of requests fulfilled
  size_t FlushPendingReceives() {

    size_t num_received{};
    while( TryReceive() ) ++num_received;

    return num_received;

  }

public:

  IprobeDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_
  ) : address(address_)
  , back_end(back_end_)
  { ; }

  ~IprobeDuct() {
    FlushPendingReceives();
  }

  [[noreturn]] bool TryPut(const T&) const {
    throw "Put called on IprobeDuct";
  }

  /**
   * TODO.
   *
   */
  [[noreturn]] bool TryFlush() const { throw "Flush called on IprobeDuct"; }

  /**
   * TODO.
   *
   * @param num_requested TODO.
   * @return number items consumed.
   */
  size_t TryConsumeGets(const size_t num_requested) {

    size_t requested_countdown{ num_requested };

    while ( requested_countdown && TryReceive() ) --requested_countdown;

    return num_requested - requested_countdown;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return buffer; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { return buffer; }

  static std::string GetName() { return "IprobeDuct"; }

  static constexpr bool CanStep() { return true; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << std::endl;
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << std::endl;
    ss << uitsl::format_member("InterProcAddress address", address) << std::endl;
    return ss.str();
  }

};

} // namespace s
} // namespace uit
