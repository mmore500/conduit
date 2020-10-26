#pragma once


#include <mpi.h>

#if defined(__EMSCRIPTEN__) && defined(OPEN_MPI)

  struct ompi_predefined_communicator_t {
    ompi_predefined_communicator_t(const int) {}
  };

  ompi_predefined_communicator_t  ompi_mpi_comm_world = {0};

#endif
