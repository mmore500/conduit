#pragma once

#include <string>

#include <unistd.h>

std::string get_hostname() {

  char res[HOST_NAME_MAX + 1];
  gethostname(res, HOST_NAME_MAX + 1);

  return res;

}
