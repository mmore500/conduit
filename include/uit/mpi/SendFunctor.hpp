#pragma once

#include <utility>

#include <mpi.h>

namespace uit {

struct SendFunctor {

  template<typename... Args>
  int operator()(Args&&... args) {
    return MPI_Send(std::forward<Args>(args)...);
  }

};

} // namespace uit
