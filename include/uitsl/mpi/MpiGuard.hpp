#pragma once

#include <functional>

#include "mpi_utils.hpp"
#include "audited_routines.hpp"

namespace uitsl {

struct MpiGuard {

  MpiGuard() { uitsl::mpi_init(); }

  ~MpiGuard() { UITSL_Finalize(); }

};

} // namespace uitsl
