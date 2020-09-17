#pragma once

#include <functional>

#include "mpi_utils.hpp"
#include "audited_routines.hpp"

namespace uitsl {

struct MpiMultithreadGuard {

  MpiMultithreadGuard() { uitsl::mpi_init_multithread(); }

  ~MpiMultithreadGuard() { UITSL_Finalize(); }

};

} // namespace uitsl
