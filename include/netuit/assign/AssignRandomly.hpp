#pragma once

#include <functional>
#include <stddef.h>

#include "../../third-party/Empirical/source/base/vector.h"
#include "../../third-party/Empirical/source/tools/random_utils.h"

namespace uit {
// TODO assign_randomly
// make a vector of IDs, shuffle it, capture it,
// then return result of indexing into it

template<typename RETURN_TYPE>
struct AssignRandomly {
  const size_t num_threads;
  emp::Random& rand;
  emp::vector<RETURN_TYPE> ids;

  AssignRandomly(
    const size_t num_threads_
  , emp::Random& rand_
  ) : num_threads(num_threads_)
    , rand(rand_)
    , ids(emp::GetPermutation(rand, num_threads))
  { ; }

  RETURN_TYPE operator()(const size_t & node_id) {
    return ids[node_id % num_threads];
  }

};
};