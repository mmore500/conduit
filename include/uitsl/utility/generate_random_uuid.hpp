#pragma once
#ifndef UITSL_UTILITY_GENERATE_RANDOM_UUID_HPP_INCLUDE
#define UITSL_UTILITY_GENERATE_RANDOM_UUID_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <functional>
#include <random>

#include "../../../third-party/stduuid/include/uuid.h"

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

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_GENERATE_RANDOM_UUID_HPP_INCLUDE
