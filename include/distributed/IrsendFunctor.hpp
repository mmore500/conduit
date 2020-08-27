#pragma once

#include <utility>

#include <mpi.h>

namespace uit {

struct IrsendFunctor {

  template<typename... Args>
  int operator()(Args&&... args) {
    return MPI_Irsend(std::forward<Args>(args)...);
  }

};

} //namespace uit
