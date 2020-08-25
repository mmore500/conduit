#pragma once

#include <functional>
#include <stddef.h>

namespace uit {

template<typename RETURN_TYPE>
struct AssignSegregated {

  RETURN_TYPE operator()(const size_t & node_id) {
    return node_id;
  }

};

};
