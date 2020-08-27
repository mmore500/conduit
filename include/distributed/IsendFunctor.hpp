#pragma once

#include <utility>

#include <mpi.h>

namespace uit {

struct IsendFunctor {

  template<typename... Args>
  int operator()(Args&&... args) {
    return MPI_Isend(std::forward<Args>(args)...);
  }

};

} // namespace uit
