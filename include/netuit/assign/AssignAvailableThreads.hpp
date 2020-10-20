#pragma once
#ifndef UITSL_PARALLEL_ASSIGN_UTILS_HPP_INCLUDE
#define UITSL_PARALLEL_ASSIGN_UTILS_HPP_INCLUDE

#include "../../uitsl/parallel/thread_utils.hpp"

namespace netuit {

struct AssignAvailableThreads {

  uitsl::thread_id_t operator()(const size_t & node_id) {
    return node_id % uitsl::get_nproc();
  }

};

} // namespace netuit

#endif // #ifndef UITSL_PARALLEL_ASSIGN_UTILS_HPP_INCLUDE
