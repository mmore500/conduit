#pragma once

#include <string>

extern "C" {
#include <metis.h>
}

#include "../utility/numeric_cast.hpp"

namespace uit::metis {
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
}
