#pragma once

#include <functional>
#include <utility>
#include <stddef.h>

#include "../../../third-party/Empirical/source/base/vector.h"

#include "../../uitsl/debug/EnumeratedFunctor.hpp"
#include "../../uitsl/parallel/thread_utils.hpp"
#include "../../uitsl/mpi/mpi_utils.hpp"
#include "../topology/Topology.hpp"

namespace uit {

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
    const auto thread_assign = subtopo.Optimize(threads_per_proc);
    for (size_t i = 0; i < subtopo.GetSize(); ++i) {
      ret[subtopo.GetCanonicalNodeID(i)] = thread_assign[i];
    }
  }

  return ret;
}

// TODO assign_chunkily (rename)
// arrange into n-dimensional volume
// then divvy into n-dimensional subcubes
// (special case of assign_contiguously)

std::pair<
  uitsl::EnumeratedFunctor<netuit::Topology::node_id_t, uitsl::proc_id_t>,
  uitsl::EnumeratedFunctor<netuit::Topology::node_id_t, uitsl::thread_id_t>
> GenerateMetisAssignments (
  const size_t num_procs,
  const size_t threads_per_proc,
  const netuit::Topology& topology
) {
  uitsl::EnumeratedFunctor<netuit::Topology::node_id_t, uitsl::proc_id_t> proc_assigner{
    topology.Optimize(num_procs)
  };

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
};
