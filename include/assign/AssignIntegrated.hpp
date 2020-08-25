#pragma once

#include <functional>
#include <stddef.h>

namespace uit {

template<typename RETURN_TYPE>
struct AssignIntegrated {
  RETURN_TYPE operator()(const size_t & node_id) {
    return 0;
  }
};

};