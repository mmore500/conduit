#pragma once
#ifndef UITSL_MPI_MPI_AUDIT_HPP_INCLUDE
#define UITSL_MPI_MPI_AUDIT_HPP_INCLUDE

#include <mpi.h>

#include "../../uit_emp/base/assert.hpp"

#include "name_mpi_error.hpp"

#include "../../uit_emp/vendorization/push_assert_macros.hh"
namespace uitsl {

inline void mpi_audit(const int err) {

  emp_assert(err == MPI_SUCCESS, err, uitsl::name_mpi_error(err));

}

} // namespace uitsl
#include "../../uit_emp/vendorization/pop_assert_macros.hh"

#endif // #ifndef UITSL_MPI_MPI_AUDIT_HPP_INCLUDE
