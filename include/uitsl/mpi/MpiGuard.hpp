#pragma once
#ifndef UITSL_MPI_MPIGUARD_HPP_INCLUDE
#define UITSL_MPI_MPIGUARD_HPP_INCLUDE

#include <functional>

#include "audited_routines.hpp"
#include "mpi_utils.hpp"

namespace uitsl {

struct MpiGuard {

  MpiGuard() { uitsl::mpi_init(); }

  ~MpiGuard() { UITSL_Finalize(); }

};

} // namespace uitsl

#endif // #ifndef UITSL_MPI_MPIGUARD_HPP_INCLUDE
