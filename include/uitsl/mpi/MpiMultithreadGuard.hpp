#pragma once
#ifndef UITSL_MPI_MPIMULTITHREADGUARD_HPP_INCLUDE
#define UITSL_MPI_MPIMULTITHREADGUARD_HPP_INCLUDE

#include <functional>

#include "audited_routines.hpp"
#include "mpi_utils.hpp"

namespace uitsl {

struct MpiMultithreadGuard {

  MpiMultithreadGuard() { uitsl::mpi_init_multithread(); }

  ~MpiMultithreadGuard() { UITSL_Finalize(); }

};

} // namespace uitsl

#endif // #ifndef UITSL_MPI_MPIMULTITHREADGUARD_HPP_INCLUDE
