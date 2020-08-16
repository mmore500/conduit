#pragma once

#include "mpi_utils.hpp"

namespace uit {

struct AssignAvailableProcs {

  uit::proc_id_t operator()(const size_t & node_id) {
    return node_id % uit::get_nprocs();
  }

};

} // namespace uit
