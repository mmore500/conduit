#pragma once

#include <iostream>
#include <functional>

#include <mpi.h>

#include "../mpi/mpi_utils.hpp"

namespace uitsl {

void print_separator(const proc_id_t rank) {
  std::cout << "======================" << std::endl;
  std::cout << "\\/ \\/ Rank " << rank << " \\/ \\/" << std::endl;
  std::cout << "======================" << std::endl;
}

//TODO add a thread do successively and a combined do successively
template<
  typename TaskFunction,
  typename BeforeTaskFunction=std::function<void(const proc_id_t)>
>
void do_successively(
  TaskFunction&& task,
  BeforeTaskFunction&& before_task=[](const proc_id_t rank){},
  MPI_Comm comm=MPI_COMM_WORLD
) {
  for (proc_id_t rank = 0; rank < uitsl::get_nprocs(comm); ++rank) {
    if (rank == uitsl::get_rank(comm)) {
      before_task(rank);
      task();
    }
    UITSL_Barrier(comm);
  }
}

} // namespace uitsl
