#pragma once

#include <functional>
#include <stddef.h>

namespace uit {
// TODO assign_chunkily (rename)
// arrange into n-dimensional volume
// then divvy into n-dimensional subcubes
// (special case of assign_contiguously)
template<typename RETURN_TYPE>
struct AssignSubspace {
  const size_t num_spaces;
  const size_t num_subspaces;
  const size_t subspaces_per_space;

  AssignSubspace(
    const size_t num_spaces_
  , const size_t num_subspaces_
  , const size_t subspaces_per_space_
  ) : num_spaces(num_spaces_)
    , num_subspaces(num_subspaces_)
    , subspaces_per_space(subspaces_per_space_)
  { ; }

  RETURN_TYPE operator()(const size_t & node_id) {
    size_t ret = node_id % num_spaces;
    ret += (node_id / num_spaces) / subspaces_per_space;
    return ret;
  }

};
};