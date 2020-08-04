#pragma once

#include <functional>
#include <stddef.h>

namespace uit {

template<typename RETURN_TYPE>
struct AssignIntegrated {
  RETURN_TYPE operator()(const size_t & node_id) {
    return 0;
  }
};

template<typename RETURN_TYPE>
struct AssignSegregated {

  RETURN_TYPE operator()(const size_t & node_id) {
    return node_id;
  }

};


template<typename RETURN_TYPE>
struct AssignContiguously {

  const size_t num_threads;
  const size_t num_nodes;

  AssignContiguously(
    const size_t num_threads_,
    const size_t num_nodes_
  ) : num_threads(num_threads_)
  , num_nodes(num_nodes_)
  { ; }

  RETURN_TYPE operator()(const size_t & node_id) {
    return node_id * num_threads / num_nodes;
  }

};

template<typename RETURN_TYPE>
struct AssignRoundRobin {

  const size_t num_threads;

  AssignRoundRobin(
    const size_t num_threads_
  ) : num_threads(num_threads_)
  { ; }

  RETURN_TYPE operator()(const size_t & node_id) {
    return node_id % num_threads;
  }

};

}
// TODO assign_randomly
// make a vector of IDs, shuffle it, capture it,
// then return result of indexing into it

// TODO assign_chunkily (rename)
// arrange into n-dimensional volume
// then divvy into n-dimensional subcubes
// (special case of assign_contiguously)
