#pragma once

#include <list>

#include "../conduit/Inlet.h"
#include "../conduit/Outlet.h"
#include "../conduit/pipe_utils.h"

template<typename T, size_t N=DEFAULT_BUFFER>
class InputLink {
  Outlet<T, N> input;
  const size_t pipe_id;

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

  /*
  * goal
  * nodes: -> 0 -> 1 -> 2 -> 3 ->
  *
  * node 0: input from 3 & output to 1
  * node 1: input from 0 & output to 2
  * node 2: input from 1 & output to 3
  * node 3: input from 2 & output to 0
  *
  * node 0: outlet 3 & inlet 0
  * node 1: outlet 0 & inlet 1
  * node 2: outlet 1 & inlet 2
  * node 3: outlet 2 & inlet 3
  */


  std::list<OutputLink<T, N>> inlets;
  std::list<InputLink<T, N>> outlets;

  for (size_t pipe_id = 0; pipe_id < cardinality; ++pipe_id) {
    auto pipe = make_pipe<T, N>();
    auto & [inlet, outlet] = pipe;
    inlets.push_back({inlet, pipe_id});
    outlets.push_back({outlet, pipe_id});
  }

  /*
  * before rotate
  * inlets:  0 1 2 3  (output from nodes)
  *          | | | |
  * outlets: 0 1 2 3  (input to nodes)
  */

  // rotate outlets right by one
  if (cardinality) outlets.splice(
    std::begin(outlets),
    outlets,
    std::prev(std::end(outlets))
  );

  /*
  * after rotate
  * nodes:   0 1 2 3
  * inlets:  0 1 2 3   (output from nodes)
  *         \ \ \ \ \
  * outlets: 3 0 1 2   (input to nodes)
  * nodes:   0 1 2 3
  */

  mesh_t<T, N> res;

  auto inlet_iterator = std::begin(inlets);
  auto outlet_iterator = std::begin(outlets);
  for (
    ;
    inlet_iterator != std::end(inlets) && outlet_iterator != std::end(outlets);
    ++inlet_iterator, ++outlet_iterator
  ) {
    res.push_back(io_bundle_t<T, N>{
      {*outlet_iterator},
      {*inlet_iterator}
    });
  }

  return res;

}

template<typename T, size_t N=DEFAULT_BUFFER>
mesh_t<T, N> make_dyadic_mesh(const size_t cardinality) {

  mesh_t<T, N> res;

  size_t pipe_id_counter{};

  for (size_t dyad = 0; dyad < cardinality/2; ++dyad) {

      auto forward_pipe = make_pipe<T, N>();
      auto & [forward_inlet, forward_outlet] = forward_pipe;
      const size_t forward_id = pipe_id_counter++;

      auto backward_pipe = make_pipe<T, N>();
      auto & [backward_inlet, backward_outlet] = backward_pipe;
      const size_t backward_id = pipe_id_counter++;

      res.push_back(io_bundle_t<T, N>{
        {{backward_outlet, backward_id}},
        {{forward_inlet, forward_id}}
      });
      res.push_back(io_bundle_t<T, N>{
        {{forward_outlet, forward_id}},
        {{backward_inlet, backward_id}}
      });

  }

  // for odd cardinality, add a loop pipe
  if (cardinality%2) {
    auto self_pipe = make_pipe<T, N>();
    auto & [self_inlet, self_outlet] = self_pipe;
    const size_t self_id = pipe_id_counter++;

    res.push_back(io_bundle_t<T, N>{
      {{self_outlet, self_id}},
      {{self_inlet, self_id}}
    });
  };

  return res;

}

template<typename T, size_t N=DEFAULT_BUFFER>
mesh_t<T, N> make_loop_mesh(const size_t cardinality) {

  mesh_t<T, N> res;

  for (size_t pipe_id = 0; pipe_id < cardinality; ++pipe_id) {
    auto self_pipe = make_pipe<T, N>();
    auto & [self_inlet, self_outlet] = self_pipe;

    res.push_back(io_bundle_t<T, N>{
      {{self_outlet, pipe_id}},
      {{self_inlet, pipe_id}}
    });
  };

  return res;

}


template<typename T, size_t N=DEFAULT_BUFFER>
mesh_t<T, N> make_producer_consumer_mesh(const size_t cardinality) {

  if (cardinality == 1) return make_loop_mesh<T, N>(cardinality);

  mesh_t<T, N> res;

  size_t pipe_id_counter{};

  for (size_t pair = 0; pair < cardinality/2; ++pair) {

      auto forward_pipe = make_pipe<T, N>();
      auto & [forward_inlet, forward_outlet] = forward_pipe;
      const size_t forward_id = pipe_id_counter++;

      res.push_back(io_bundle_t<T, N>{
        {},
        {{forward_inlet, forward_id}}
      });
      res.push_back(io_bundle_t<T, N>{
        {{forward_outlet, forward_id}},
        {}
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
