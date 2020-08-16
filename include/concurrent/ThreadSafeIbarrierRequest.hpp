#pragma once

#include <mpi.h>
#include <mutex>

#include "../distributed/IbarrierRequest.hpp"

namespace uit {

/**
 * Post a MPI Ibarrier and easily test for its completion.
 */
class ThreadSafeIbarrierRequest {

  /// Request handle corresponding to Ibarrier call.
  std::optional<uit::IbarrierRequest> request{ std::nullopt };

  static inline std::mutex mutex{};

public:

  /**
   * @param comm Set of MPI processes to participate in barrier.
   */
  ThreadSafeIbarrierRequest(const MPI_Comm comm=MPI_COMM_WORLD) {
    const std::lock_guard guard{ mutex };
    request.emplace(comm);
  }

  /**
   * Test if all processes have reached barrier.
   *
   * @returns true if all processes have reached barrier.
   */
  bool IsComplete() const {
    const std::lock_guard guard{ mutex };
    return request->IsComplete();
  }

};

} // namespace uit
