#pragma once

#include <mpi.h>

#include "../mpi/audited_routine_aliases.hpp"
#include "../mpi/mpi_utils.hpp"

namespace uit {

/**
 * Post a MPI Ibarrier and easily test for its completion.
 */
class IbarrierRequest {

  /// Request handle corresponding to Ibarrier call.
  MPI_Request request;

public:

  /**
   * @param comm Set of MPI processes to participate in barrier.
   */
  IbarrierRequest(const MPI_Comm comm=MPI_COMM_WORLD) {
    UIT_Ibarrier(comm, &request);
  }

  /**
   * Test if all processes have reached barrier.
   *
   * @returns true if all processes have reached barrier.
   */
  bool IsComplete() const { return uit::test_completion(request); }

};

} // namespace uit
