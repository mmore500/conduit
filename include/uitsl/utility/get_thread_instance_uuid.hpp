#pragma once
#ifndef UITSL_UTILITY_GET_THREAD_INSTANCE_UUID_HPP_INCLUDE
#define UITSL_UTILITY_GET_THREAD_INSTANCE_UUID_HPP_INCLUDE

#include "../../../third-party/stduuid/include/uuid.h"

#include "generate_random_uuid.hpp"

namespace uitsl {

uuids::uuid get_thread_instance_uuid() {
  thread_local auto uuid = uitsl::generate_random_uuid();
  return uuid;
}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_GET_THREAD_INSTANCE_UUID_HPP_INCLUDE
