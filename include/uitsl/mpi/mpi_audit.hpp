#pragma once
#ifndef UITSL_MPI_MPI_AUDIT_HPP_INCLUDE
#define UITSL_MPI_MPI_AUDIT_HPP_INCLUDE

#include <mpi.h>

#include "../../uitsl/debug/uitsl_assert.hpp"

#include "name_mpi_error.hpp"

namespace uitsl {

inline void mpi_audit(const int err) {

  uitsl_assert(err == MPI_SUCCESS, err << uitsl::name_mpi_error(err));

}

} // namespace uitsl

#endif // #ifndef UITSL_MPI_MPI_AUDIT_HPP_INCLUDE
