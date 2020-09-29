#pragma once
#ifndef UITSL_DISTRIBUTED_ASSIGN_UTILS_HPP_INCLUDE
#define UITSL_DISTRIBUTED_ASSIGN_UTILS_HPP_INCLUDE

#include "../mpi/mpi_utils.hpp"

namespace uitsl {

struct AssignAvailableProcs {

  uitsl::proc_id_t operator()(const size_t & node_id) {
    return node_id % uitsl::get_nprocs();
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_DISTRIBUTED_ASSIGN_UTILS_HPP_INCLUDE
