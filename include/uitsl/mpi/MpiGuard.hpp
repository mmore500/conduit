#pragma once

#include <functional>

#include "audited_routines.hpp"
#include "mpi_utils.hpp"

namespace uitsl {

struct MpiGuard {

  MpiGuard() { uitsl::mpi_init(); }

  ~MpiGuard() { UITSL_Finalize(); }

};

} // namespace uitsl
