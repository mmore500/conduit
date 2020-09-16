#pragma once

#include <assert.h>
#include <limits>
#include <numeric>
#include <stddef.h>

#include <mpi.h>

#include "../../../third-party/Empirical/source/base/array.h"
#include "../../../third-party/Empirical/source/base/errors.h"

#include "../utility/safe_cast.hpp"
#include "../utility/print_utils.hpp"

#include "audited_routines.hpp"
#include "comm_utils.hpp"
#include "group_utils.hpp"
#include "proc_id_t.hpp"
#include "request_utils.hpp"
#include "status_utils.hpp"

namespace uit {

void mpi_init_multithread(int *argc, char ***argv) {
  int res{};
  UIT_Init_thread(argc, argv, MPI_THREAD_MULTIPLE, &res);
  if (res < MPI_THREAD_MULTIPLE) emp::NotifyWarning(
    "MPI implementation may not provide full multithread support."
  );
}

void mpi_init_multithread() {
  int argc{};
  mpi_init_multithread(&argc, nullptr);
}

void mpi_init() {
  int argc{};
  UIT_Init(&argc, nullptr);
}

} // namespace uit
