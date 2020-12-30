#pragma once
#ifndef UITSL_MPI_MPIGUARD_HPP_INCLUDE
#define UITSL_MPI_MPIGUARD_HPP_INCLUDE

#include <functional>

#include <mpi.h>

#include "../debug/err_verify.hpp"

namespace uitsl {

struct MpiGuard {

  MpiGuard() {
    int argc{};
    uitsl::err_verify( MPI_Init(&argc, nullptr) );
  }

  ~MpiGuard() { uitsl::err_verify( MPI_Finalize() ); }

};

} // namespace uitsl

#endif // #ifndef UITSL_MPI_MPIGUARD_HPP_INCLUDE
