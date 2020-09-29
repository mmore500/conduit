#pragma once
#ifndef UITSL_PARALLEL_ASSIGN_UTILS_HPP_INCLUDE
#define UITSL_PARALLEL_ASSIGN_UTILS_HPP_INCLUDE

#include "thread_utils.hpp"

namespace uitsl {

struct AssignAvailableThreads {

  uitsl::thread_id_t operator()(const size_t & node_id) {
    return node_id % uitsl::get_nproc();
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_PARALLEL_ASSIGN_UTILS_HPP_INCLUDE
