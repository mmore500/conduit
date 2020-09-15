#pragma once

#include <functional>

#include "mpi_utils.hpp"
#include "audited_routines.hpp"

namespace uit {

struct MpiMultithreadGuard {

  MpiMultithreadGuard() { uit::mpi_init_multithread(); }

  ~MpiMultithreadGuard() { UIT_Finalize(); }

};

} // namespace uit
