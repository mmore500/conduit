#pragma once

#include "Inlet.h"
#include "Outlet.h"
#include "pipe_utils.h"

template<typename T, size_t N=DEFAULT_BUFFER>
class InputLink {
  Outlet<T, N> input;
  size_t pipe_id;

public:

  InputLink(
    const Outlet<T, N>& input_,
    const size_t pipe_id_
  ) : input(input_)
  , pipe_id(pipe_id_)
  { ; }

  operator Outlet<T, N>() const { return input; }

  operator size_t() const { return pipe_id; }

  Outlet<T, N> & GetInput() { return input; }

  size_t GetPipeID() const { return pipe_id; }

};

template<typename T, size_t N=DEFAULT_BUFFER>
class OutputLink {
  Inlet<T, N> output;
  const size_t pipe_id;

public:

  OutputLink(
    const Inlet<T, N>& output_,
    const size_t pipe_id_
  ) : output(output_)
  , pipe_id(pipe_id_)
  { ; }

  operator Inlet<T, N>() const { return output; }

  operator size_t() const { return pipe_id; }

  Inlet<T, N> & GetOutput() { return output; }

  size_t GetPipeID() const { return pipe_id; }

};

template<typename T, size_t N=DEFAULT_BUFFER>
struct io_bundle_t {
  emp::vector<InputLink<T, N>> inputs;
  emp::vector<OutputLink<T, N>> outputs;
};

template<typename T, size_t N=DEFAULT_BUFFER>
using mesh_t = emp::vector<
  io_bundle_t<T, N>
>;

template<typename T, size_t N=DEFAULT_BUFFER>
mesh_t<T, N> make_ring_mesh(const size_t cardinality) {

  std::vector<OutputLink<T, N>> inlets;
  std::vector<InputLink<T, N>> outlets;

  for (size_t i = 0; i < cardinality; ++i) {
    auto res = make_pipe<T, N>();
    auto & [inlet, outlet] = res;
    inlets.push_back({inlet, i});
    outlets.push_back({outlet, i});
  }

  // before rotate
  // outlets: 0 1 2 3  (inputs)
  //          | | | |
  // inlets:  0 1 2 3  (outputs)

  if (cardinality) std::rotate(
    std::rbegin(outlets),
    std::rbegin(outlets) + 1,
    std::rend(outlets)
  );

  // after rotate
  // outlets:   0 1 2 3  (inputs)
  //           \ \ \ \
  // inlets:    3 0 1 2  (outputs)

  mesh_t<T, N> res;

  for (size_t i = 0; i < cardinality; ++i) {
    res.push_back(io_bundle_t<T, N>{
      {outlets[i]},
      {inlets[i]}
    });
  }

  return res;

}

// TODO
// * make_random_mesh
//   * degree is argument
// * make_toroidal_mesh (make ring mesh is special case of this)
//   * dimension is argument
// * make_grid_mesh (like toroidal mesh, but no wraparound)
//   * dimension is argument
// * make_complete_mesh

template<typename RETURN_TYPE>
std::function<RETURN_TYPE(size_t)> assign_contiguously(
  const size_t num_threads,
  const size_t num_nodes
) {
  return [=](const auto & node_id){
    return node_id * num_threads / num_nodes;
  };
};

template<typename RETURN_TYPE>
std::function<RETURN_TYPE(size_t)> assign_round_robin(
  const size_t num_threads
) {
  return [=](const auto & node_id){
    return node_id % num_threads;
  };
};

// TODO assign_randomly
// make a vector of IDs, shuffle it, capture it,
// then return result of indexing into it

// TODO assign_chunkily (rename)
// arrange into n-dimensional volume
// then divvy into n-dimensional subcubes
// (special case of assign_contiguously)

// TODO make a util to get only values corresponding
// to your thread's or your proc's indices
// out of a container
// maybe make a class MeshDelegator that divies everything up in constructor
// then you can iterate through sub-vectors?
