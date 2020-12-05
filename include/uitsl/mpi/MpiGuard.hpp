#pragma once
#ifndef UITSL_MPI_MPIGUARD_HPP_INCLUDE
#define UITSL_MPI_MPIGUARD_HPP_INCLUDE

#include <functional>

namespace uitsl {

struct MpiGuard {

  MpiGuard() {
    int argc{};
    const auto res = MPI_Init(&argc, nullptr);
    assert(res == 0);
  }

  ~MpiGuard() {
    const auto res = MPI_Finalize();
    assert(res == 0);
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_MPI_MPIGUARD_HPP_INCLUDE
