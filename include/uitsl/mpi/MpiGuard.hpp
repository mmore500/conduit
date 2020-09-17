#pragma once

#include <functional>

#include "mpi_utils.hpp"
#include "audited_routines.hpp"

namespace uit {

struct MpiGuard {

  MpiGuard() { uit::mpi_init(); }

  ~MpiGuard() { UIT_Finalize(); }

};

} // namespace uit
