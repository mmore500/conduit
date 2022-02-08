#pragma once
#ifndef UITSL_UTILITY_GET_PROC_INSTANCE_UUID_HPP_INCLUDE
#define UITSL_UTILITY_GET_PROC_INSTANCE_UUID_HPP_INCLUDE

#include "../../../third-party/stduuid/include/uuid.h"

#include "generate_random_uuid.hpp"

namespace uitsl {

uuids::uuid get_proc_instance_uuid() {
  static auto uuid = uitsl::generate_random_uuid();
  return uuid;
}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_GET_PROC_INSTANCE_UUID_HPP_INCLUDE
