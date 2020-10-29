#pragma once
#ifndef NETUIT_ASSIGN_GENERATEMETISASSIGNMENTS_HPP_INCLUDE
#define NETUIT_ASSIGN_GENERATEMETISASSIGNMENTS_HPP_INCLUDE

#include <functional>
#include <stddef.h>
#include <utility>

#ifndef __EMSCRIPTEN__
#include <metis.h>
#endif

#include "../../../third-party/Empirical/source/base/vector.h"

#include "../../uitsl/debug/EnumeratedFunctor.hpp"
#include "../../uitsl/debug/audit_cast.hpp"
#include "../../uitsl/mpi/mpi_utils.hpp"
#include "../../uitsl/parallel/thread_utils.hpp"

#include "../topology/Topology.hpp"

namespace netuit {

/// Apply METIS' K-way partitioning algorithm to subdivide topology
/// @param parts number of parts to subdivide topology into
/// @param topology topology to subdivide
/// @return vector indicating what partition each vertex should go into
emp::vector<idx_t> PartitionMetis(
  const size_t num_parts, const netuit::Topology& topology
) {

  // set up result vector
  emp::vector<idx_t> result( topology.GetSize() );

  #ifndef __EMSCRIPTEN___
  // set up variables
  idx_t nodes = topology.GetSize();
  idx_t n_cons = 1;
  idx_t parts = uitsl::audit_cast<idx_t>( num_parts );
  idx_t objval;

  // get topology as CSR
  auto [xadj, adjacency] = topology.AsCSR();

  // use default options
  idx_t options[METIS_NOPTIONS];
  METIS_SetDefaultOptions(options);

  // call partitioning algorithm
  const int status = METIS_PartGraphKway(
    &nodes, // number of vertices in the graph
    &n_cons, // number of balancing constraints.
    xadj.data(), // array of node indexes into adjacency[]
    adjacency.data(), // array of adjacenct nodes for every node
    nullptr, // weights of nodes
    nullptr, // size of nodes for total comunication value
    nullptr, // weights of edges
    &parts, // number of parts to partition the graph into
    nullptr, // weight for each partition and constraint
    nullptr, // allowed load imbalance tolerance for each constraint
    nullptr, // array of options
    &objval, // edge-cut or total comm volume of the solution
    result.data() // partition vector of the graph
  );

  uitsl::metis::verify(status);
  #endif

  return result;
}

/// This function is used to get subtopologies made up of all
/// the neighbors of a node in a given topology, for all nodes.
/// @param[in] topo Topology to get subtopologies of.
/// @param[in] assigner Functor of node ids to proc ids.
/// @return Unordered map of proc ids to subtopologies.
std::unordered_map<uitsl::proc_id_t, netuit::Topology> GetSubTopologies(
  const netuit::Topology& topo,
  const uitsl::EnumeratedFunctor<netuit::Topology::node_id_t, uitsl::proc_id_t>& assigner
) {
  std::unordered_map<uitsl::proc_id_t, netuit::Topology> subtopos;

  std::unordered_map<
    uitsl::proc_id_t,
    std::unordered_set<size_t>
  > node_map;
  for (size_t node_id = 0; node_id < assigner.GetSize(); ++node_id) {
    node_map[assigner(node_id)].insert(node_id);
  }

  // todo: only run for current process?
  for (const auto& [proc_id, nodes] : node_map) {
    // make subtopology
    subtopos[proc_id] = topo.GetSubTopology(nodes);
  }

  return subtopos;
}

// todo: rename
std::unordered_map<size_t, uitsl::thread_id_t> Shim(
  const std::unordered_map<uitsl::proc_id_t, netuit::Topology>& proc_map,
  const size_t threads_per_proc
) {
  std::unordered_map<size_t, uitsl::thread_id_t> ret;

  for (const auto& [proc_id, subtopo] : proc_map) {
    const auto thread_assign = PartitionMetis( threads_per_proc, subtopo );
    for (size_t i = 0; i < subtopo.GetSize(); ++i) {
      ret[subtopo.GetCanonicalNodeID(i)] = thread_assign[i];
    }
  }

  return ret;
}


/// This function returns a pair of functors determining thread and process
/// assignments, from a (hopefully optimal) k-way partitioning as returned by METIS.
/// @param[in] num_procs Number of processes.
/// @param[in] threads_per_proc Number of threads per process.
/// @param[in] topology Topology to partition.
/// @return std::pair of process and thread assignments. *
std::pair<
  uitsl::EnumeratedFunctor<netuit::Topology::node_id_t, uitsl::proc_id_t>,
  uitsl::EnumeratedFunctor<netuit::Topology::node_id_t, uitsl::thread_id_t>
> GenerateMetisAssignments (
  const size_t num_procs,
  const size_t threads_per_proc,
  const netuit::Topology& topology
) {
  // make sure topology isn't empty
  if (topology.GetSize() == 0) return {};

  uitsl::EnumeratedFunctor<netuit::Topology::node_id_t, uitsl::proc_id_t> proc_assigner{ PartitionMetis(num_procs, topology) };

  uitsl::EnumeratedFunctor<netuit::Topology::node_id_t, uitsl::thread_id_t> thread_assigner{
    Shim(
      GetSubTopologies(topology, proc_assigner),
      threads_per_proc
    )
  };

  return {
    proc_assigner,
    thread_assigner
  };
}

} // namespace netuit

#endif // #ifndef NETUIT_ASSIGN_GENERATEMETISASSIGNMENTS_HPP_INCLUDE
