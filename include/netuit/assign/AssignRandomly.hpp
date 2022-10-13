#pragma once
#ifndef NETUIT_ASSIGN_ASSIGNRANDOMLY_HPP_INCLUDE
#define NETUIT_ASSIGN_ASSIGNRANDOMLY_HPP_INCLUDE

#include <functional>
#include <stddef.h>
#include <vector>

#include "../../uit_emp/math/Random.hpp"
#include "../../uit_emp/math/random_utils.hpp"

namespace netuit {

template<typename RETURN_TYPE>
struct AssignRandomly {
  const size_t num_threads;
  uit_emp::Random& rand;
  std::vector<RETURN_TYPE> ids;

  AssignRandomly(
    const size_t num_threads_
  , uit_emp::Random& rand_
  ) : num_threads(num_threads_)
    , rand(rand_)
    , ids(uit_emp::GetPermutation(rand, num_threads))
  { ; }

  RETURN_TYPE operator()(const size_t & node_id) {
    return ids[node_id % num_threads];
  }

};

} // namespace netuit

#endif // #ifndef NETUIT_ASSIGN_ASSIGNRANDOMLY_HPP_INCLUDE
