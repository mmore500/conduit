#pragma once

#include <functional>

#include "mpi_utils.hpp"
#include "audited_routine_aliases.hpp"

namespace uit {

struct MPIGuard {

  MPIGuard() { uit::mpi_init(); }

  ~MPIGuard() { UIT_Finalize(); }

};

} // namespace uit
