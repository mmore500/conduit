#pragma once
#ifndef UITSL_DEBUG_METIS_UTILS_HPP_INCLUDE
#define UITSL_DEBUG_METIS_UTILS_HPP_INCLUDE

#include <string>

#include <metis.h>

namespace uitsl::metis {
void verify(const int status) {
  std::string name;

  switch(status) {
    case METIS_OK:
      name = "METIS_OK";
      break;
    case METIS_ERROR_INPUT:
      name = "METIS_ERROR_INPUT";
      break;
    case METIS_ERROR_MEMORY:
      name = "METIS_ERROR_MEMORY";
      break;
    case METIS_ERROR:
      name = "METIS_ERROR";
      break;
    default:
      name = "unknown";
      break;
  }
  emp_assert(status == METIS_OK, status, name);
}

} // namespace uitsl::metis

#endif // #ifndef UITSL_DEBUG_METIS_UTILS_HPP_INCLUDE
