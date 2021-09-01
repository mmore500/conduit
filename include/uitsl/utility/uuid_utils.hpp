#pragma once
#ifndef UITSL_UTILITY_UUID_UTILS_HPP_INCLUDE
#define UITSL_UTILITY_UUID_UTILS_HPP_INCLUDE

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <random>
#include <string>
#include <string_view>

#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/error.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/stduuid/include/uuid.h"

#include "../mpi/comm_utils.hpp"
#include "../mpi/mpi_init_utils.hpp"
#include "../polyfill/filesystem.hpp"

namespace uitsl {

uuids::uuid generate_random_uuid() {
  thread_local auto random_generator = [](){
    // workaround for error encountered when multiprocesses share same node
    // > terminate called after throwing an instance of 'std::runtime_error'
    // > what():  random_device: rdseed failed
    // see https://github.com/xdspacelab/openvslam/issues/319#issuecomment-630225541
    // see https://en.cppreference.com/w/cpp/numeric/random/random_device/random_device
    std::random_device rd("rdrand");
    auto seed_data = std::array<int, std::mt19937::state_size> {};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    return std::mt19937(seq);
  }();
  thread_local uuids::uuid_random_generator uuid_generator( random_generator );

  return uuid_generator();
}

uuids::uuid get_exec_instance_uuid() {

  static auto uuid = [](){

    uuids::uuid res;

    // try to set up res uuid using a series of fallback options...
    // 1. if not multiprocess, can use random uuid generation
    if ( !uitsl::is_multiprocess() ) res = uitsl::generate_random_uuid();
    // 2. if user provided UITSL_EXEC_INSTANCE_UUID, verify it and then use it
    else if ( const char* seed = std::getenv("UITSL_EXEC_INSTANCE_UUID") ) {
      auto parsed = uuids::uuid::from_string(seed);
      emp_always_assert(
        parsed.has_value(),
        "UITSL_EXEC_INSTANCE_UUID must be a valid uuid", seed
      );
      emp_always_assert(
        !parsed->is_nil(),
        "UITSL_EXEC_INSTANCE_UUID must be a non-nil uuid", seed, *parsed
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
      / emp::keyname::pack({
        {"a", "uitsl_exec_instance_uuid"},
        {"rank",emp::to_string( uitsl::get_proc_id() )},
        {"uid", uuids::to_string( res )},
      });
    emp_always_assert(
      !std::filesystem::exists( reservation_path ),
      reservation_path,
      uitsl::is_multiprocess(),
      std::getenv("UITSL_EXEC_INSTANCE_UUID"),
      std::getenv("PMIX_NAMESPACE")
    );

    // touch reservation_path
    // adapted from https://stackoverflow.com/a/54451555
    std::ofstream output(reservation_path);

    return res;

  }();

  return uuid;

}

uuids::uuid get_proc_instance_uuid() {
  static auto uuid = uitsl::generate_random_uuid();
  return uuid;
}

uuids::uuid get_thread_instance_uuid() {
  thread_local auto uuid = uitsl::generate_random_uuid();
  return uuid;
}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_UUID_UTILS_HPP_INCLUDE
