#pragma once

#include <functional>

#include "audited_routines.hpp"
#include "mpi_utils.hpp"

namespace uitsl {

struct MpiMultithreadGuard {

  MpiMultithreadGuard() { uitsl::mpi_init_multithread(); }

  ~MpiMultithreadGuard() { UITSL_Finalize(); }

};

} // namespace uitsl
