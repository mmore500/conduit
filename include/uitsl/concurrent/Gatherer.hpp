#pragma once

#include <algorithm>
#include <numeric>
#include <thread>

#include <mpi.h>

#include "../../../third-party/Empirical/source/base/vector.h"
#include "../../../third-party/Empirical/source/base/optional.h"

#include "../containers/safe/deque.hpp"
#include "../mpi/mpi_utils.hpp"
#include "../mpi/audited_routines.hpp"

namespace uit {

/**
 * Gather data items from across threads and processes.
 *
 * Uses a thread-safe container to record values from different threads within a
 * process. Then uses a MPI `gatherv` call to collect values from across
 * processes. Arbitrary numbers of data items may be contributed from each
 * thread and from each process.
 *
 * @tparam T Value type to be gathered.
 */
template<typename T>
class Gatherer {

  /// Data items submitted from threads in the executing process.
  uit::safe::deque<T> items;

  /// MPI datatype corresponding to @T.
  // TODO use template metaprogramming to automatically deduce this
  MPI_Datatype mpi_type;
  /// Set of MPI processes to gather values from.
  MPI_Comm comm;

  /**
   * Gather counts of data items from participating processes.
   *
   * @param root The ID of the process to gather to.
   * @returns Data item count from each participating process.
   */
  emp::vector<int> GatherCounts(const int root) {

    const int count = items.size();
    emp::vector<int> res(get_nprocs());

    UIT_Gather(
      &count, // const void *sendbuf,
      1, // int sendcount,
      MPI_INT, // MPI_Datatype sendtype,
      res.data(), // void *recvbuf,
      1, // int recvcount,
      MPI_INT, // MPI_Datatype recvtype,
      root, // int root,
      comm // MPI_Comm comm
    );

    return res;

  }

public:

  /**
   * @param mpi_type_ MPI datatype corresponding to @T.
   * @param comm_ Set of MPI processes to gather values from.
   */
  Gatherer(
    MPI_Datatype mpi_type_,
    MPI_Comm comm_=MPI_COMM_WORLD
  )
  : mpi_type(mpi_type_)
  , comm(comm_)
  { ; }

  /**
   * Add a data item to the collection that will be gathered from.
   *
   * @param item The data item to store.
   * @note This call is thread-safe.
   */
  void Put(const T& item) { items.push_back(item); }

  /**
   * Gather put data items from across threads and processes.
   *
   * @param root The process ID to gather to.
   * @returns Gathered data items if root process, otherwise std::nullopt.
   * @note All processes within @comm must make this call simultaneously.
   * @note Only a single thread from each process should make this call.
   */
  emp::optional<emp::vector<T>> Gather(const int root=0) {

    const emp::vector<int> counts = GatherCounts(root);

    // calculate where each processes' contribution should be placed
    emp::vector<int> displacements{0};
    std::partial_sum(
      std::begin(counts),
      std::end(counts),
      std::back_inserter(displacements)
    );

    // initialize buffer to hold contributed items from all processes
    const size_t num_items = std::accumulate(
      std::begin(counts),
      std::end(counts),
      0
    );
    emp::vector<T> res(num_items);

    // initialize buffer to contribute items from
    emp::vector<T> send_buffer( std::begin(items), std::end(items) );

    // do gather, contributed items are only delivered to root process
    UIT_Gatherv(
      send_buffer.data(), // const void *sendbuf
      send_buffer.size(), // int sendcount
      mpi_type, // MPI_Datatype sendtype
      res.data(), // void *recvbuf
      counts.data(), // const int *recvcounts
      displacements.data(), // const int *displs
      mpi_type, // MPI_Datatype recvtype
      root, // int root
      comm // MPI_Comm comm
    );

    // if executing process is root, return gathered items
    return root == get_rank(comm)
      ? emp::optional<emp::vector<T>>{ res }
      : std::nullopt;

  }

};

} // namespace uit
