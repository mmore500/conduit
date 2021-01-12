#pragma once
#ifndef UITSL_MPI_MPIAUTOGUARD_HPP_INCLUDE
#define UITSL_MPI_MPIAUTOGUARD_HPP_INCLUDE

#include "MpiGuard.hpp"

namespace uitsl {
namespace internal {

inline const uitsl::MpiGuard mpi_guard;

} // namespace internal
} // namespace uitsl

#endif // #ifndef UITSL_MPI_MPIAUTOGUARD_HPP_INCLUDE
