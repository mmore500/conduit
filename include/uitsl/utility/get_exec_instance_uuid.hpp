#pragma once
#ifndef UITSL_UTILITY_GET_EXEC_INSTANCE_UUID_HPP_INCLUDE
#define UITSL_UTILITY_GET_EXEC_INSTANCE_UUID_HPP_INCLUDE

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <string>
#include <string_view>

#include "../../../third-party/stduuid/include/uuid.h"

#include "../../uit_emp/base/always_assert.hpp"
#include "../../uit_emp/base/error.hpp"
#include "../../uit_emp/tools/keyname_utils.hpp"
#include "../../uit_emp/tools/string_utils.hpp"

#include "../debug/uitsl_always_assert.hpp"
#include "../mpi/comm_utils.hpp"
#include "../mpi/mpi_init_utils.hpp"
#include "../polyfill/filesystem.hpp"

#include "generate_random_uuid.hpp"

namespace uitsl {

uuids::uuid get_exec_instance_uuid() {

  static auto uuid = [](){

    uuids::uuid res;

    // try to set up res uuid using a series of fallback options...
    // 1. if not multiprocess, can use random uuid generation
    if ( !uitsl::is_multiprocess() ) res = uitsl::generate_random_uuid();
    // 2. if user provided UITSL_EXEC_INSTANCE_UUID, verify it and then use it
    else if ( const char* seed = std::getenv("UITSL_EXEC_INSTANCE_UUID") ) {
      auto parsed = uuids::uuid::from_string(seed);
      uitsl_always_assert(
        parsed.has_value(),
        "UITSL_EXEC_INSTANCE_UUID must be a valid uuid" << seed
      );
      uitsl_always_assert(
        !parsed->is_nil(),
        "UITSL_EXEC_INSTANCE_UUID must be a non-nil uuid"
          << seed << *parsed
      );
      res = *parsed;
    // 3. if PMIX_NAMESPACE is available, use that to make uuid
    } else if ( const char* seed = std::getenv("PMIX_NAMESPACE") ) {
      // is PMIX_NAMESPACE actually a good uuid seed? not sure
      // note: PMIX_NAMESPACE only seems to be provided by OpenMPI
      res = uuids::uuid_name_generator{ uuids::uuid{} }(seed);
    // 4. no good options left, fail
    } else emp_error(
      "Need env var UITSL_EXEC_INSTANCE_UUID or PMIX_NAMESPACE "
      "for get_multiprocess_exec_instance_uuid.\n"
      "Hint: call "
      "`export UITSL_EXEC_INSTANCE_UUID=\"$(cat /proc/sys/kernel/random/uuid)\""
      " _each time_ prior to launching with mpiexec."
    );

    // do a quick (non-exhaustive) check for obvious user misuse
    // i.e., that uuid is actually unique
    const auto reservation_path = std::filesystem::temp_directory_path()
      / uit_emp::keyname::pack({
        {"a", "uitsl_exec_instance_uuid"},
        {"rank",uit_emp::to_string( uitsl::get_proc_id() )},
        {"uid", uuids::to_string( res )},
      });
    uitsl_always_assert(
      !std::filesystem::exists( reservation_path ),
      reservation_path
        << uitsl::is_multiprocess()
        << std::getenv("UITSL_EXEC_INSTANCE_UUID")
        << std::getenv("PMIX_NAMESPACE")
    );

    // touch reservation_path
    // adapted from https://stackoverflow.com/a/54451555
    std::ofstream output(reservation_path);

    return res;

  }();

  return uuid;

}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_GET_EXEC_INSTANCE_UUID_HPP_INCLUDE
