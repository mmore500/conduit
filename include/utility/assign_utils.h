#pragma once

#include <functional>

namespace uit {

template<typename RETURN_TYPE>
std::function<RETURN_TYPE(size_t)> assign_integrated() {
  return [](const auto & node_id){ return 0; };
}

template<typename RETURN_TYPE>
std::function<RETURN_TYPE(size_t)> assign_segregated() {
  return [](const auto & node_id){ return node_id; };
}

template<typename RETURN_TYPE>
std::function<RETURN_TYPE(size_t)> assign_contiguously(
  const size_t num_threads,
  const size_t num_nodes
) {
  return [=](const auto & node_id){
    return node_id * num_threads / num_nodes;
  };
}

template<typename RETURN_TYPE>
std::function<RETURN_TYPE(size_t)> assign_round_robin(
  const size_t num_threads
) {
  return [=](const auto & node_id){
    return node_id % num_threads;
  };
}

}
