#pragma once

#include <functional>

#include "mpi_utils.hpp"

namespace uit {

struct MPIGuard {

  MPIGuard() { uit::mpi_init(); }

  ~MPIGuard() { uit::verify(MPI_Finalize()); }

};

} // namespace uit
