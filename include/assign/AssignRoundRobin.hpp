#pragma once

#include <functional>
#include <stddef.h>

namespace uit {

template<typename RETURN_TYPE>
struct AssignRoundRobin {

  const size_t num_threads;

  AssignRoundRobin(
    const size_t num_threads_
  ) : num_threads(num_threads_)
  { ; }

  RETURN_TYPE operator()(const size_t & node_id) {
    return node_id % num_threads;
  }

};
};