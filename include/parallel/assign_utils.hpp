#pragma once

#include "thread_utils.hpp"

namespace uit {

struct AssignAvailableThreads {

  uit::thread_id_t operator()(const size_t & node_id) {
    return node_id % uit::get_nproc();
  }

};

}
