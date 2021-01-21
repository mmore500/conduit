#pragma once

#include "uitsl/mpi/comm_utils.hpp"

#include "cfg.hpp"

size_t num_nodes() {
  return  uitsl::n_procs() * cfg.N_THREADS() * cfg.N_NODES_PER_CPU()
}
