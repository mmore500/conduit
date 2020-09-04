#pragma once

#include <utility>

#include <mpi.h>

namespace uit {

struct RsendFunctor {

  template<typename... Args>
  int operator()(Args&&... args) {
    return MPI_Rsend(std::forward<Args>(args)...);
  }

};

} // namespace uit
