#pragma once

#include "mpi.h"

#include <algorithm>
#include <numeric>
#include <thread>

#include "base/vector.h"

#include "../distributed/mpi_utils.h"

template<typename T>
class Gatherer {

  emp::vector<T> tokens;
  std::mutex mutex;

  MPI_Datatype mpi_type;
  MPI_Comm comm;

  emp::vector<int> GatherCounts(const int root) {

    const int count = tokens.size();
    emp::vector<int> res(get_nprocs());

    verify(MPI_Gather(
      &count, // const void *sendbuf,
      1, // int sendcount,
      MPI_INT, // MPI_Datatype sendtype,
      res.data(), // void *recvbuf,
      1, // int recvcount,
      MPI_INT, // MPI_Datatype recvtype,
      root, // int root,
      comm // MPI_Comm comm
    ));

    return res;

  }

public:

  Gatherer(
    MPI_Datatype mpi_type_,
    MPI_Comm comm_=MPI_COMM_WORLD
  )
  : mpi_type(mpi_type_)
  , comm(comm_)
  { ; }

  void Put(const T& token) {
    const std::lock_guard guard(mutex);
    tokens.push_back(token);
  }

  std::optional<emp::vector<T>> Gather(
    const int root=0
  ) {
    const std::lock_guard guard(mutex);

    const emp::vector<int> counts = GatherCounts(root);

    emp::vector<int> displacements{0};

    std::partial_sum(
      std::begin(counts),
      std::end(counts),
      std::back_inserter(displacements)
    );

    emp::vector<T> res(
      std::accumulate(
        std::begin(counts),
        std::end(counts),
        0
      )
    );

    verify(MPI_Gatherv(
      tokens.data(), // const void *sendbuf,
      tokens.size(), // int sendcount,
      mpi_type, // MPI_Datatype sendtype,
      res.data(), // void *recvbuf,
      counts.data(), // const int *recvcounts,
      displacements.data(), // const int *displs,
      mpi_type, // MPI_Datatype recvtype,
      root, // int root,
      comm // MPI_Comm comm
    ));

    return root == get_rank()
      ? std::optional{res}
      : std::nullopt;

  }

};
