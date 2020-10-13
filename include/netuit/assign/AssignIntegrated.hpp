#pragma once
#ifndef NETUIT_ASSIGN_ASSIGNINTEGRATED_HPP_INCLUDE
#define NETUIT_ASSIGN_ASSIGNINTEGRATED_HPP_INCLUDE

#include <functional>
#include <stddef.h>

namespace uit {

template<typename RETURN_TYPE>
struct AssignIntegrated {
  RETURN_TYPE operator()(const size_t & node_id) {
    return 0;
  }
};

} // namespace uit

#endif // #ifndef NETUIT_ASSIGN_ASSIGNINTEGRATED_HPP_INCLUDE
