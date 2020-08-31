#pragma once

#include <functional>
#include <utility>
#include <stddef.h>

#include "../../third-party/Empirical/source/base/vector.h"

#include "../utility/EnumeratedFunctor.hpp"
#include "../parallel/thread_utils.hpp"
#include "../distributed/mpi_utils.hpp"
#include "../topology/Topology.hpp"

namespace uit {

std::unordered_map<uit::proc_id_t, Topology> GetSubTopologies(
  const Topology& topo,
  const uit::EnumeratedFunctor<Topology::node_id_t, uit::proc_id_t>& assigner
) {
  std::unordered_map<uit::proc_id_t, Topology> subtopos;

  std::unordered_map<
    uit::proc_id_t,
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
std::unordered_map<size_t, uit::thread_id_t> Shim(
  const std::unordered_map<uit::proc_id_t, Topology>& proc_map,
  const size_t threads_per_proc
) {
  std::unordered_map<size_t, uit::thread_id_t> ret;

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
  uit::EnumeratedFunctor<Topology::node_id_t, uit::proc_id_t>,
  uit::EnumeratedFunctor<Topology::node_id_t, uit::thread_id_t>
> GenerateMetisAssignments (
  const size_t num_procs,
  const size_t threads_per_proc,
  const Topology& topology
) {
  uit::EnumeratedFunctor<Topology::node_id_t, uit::proc_id_t> proc_assigner{
    topology.Optimize(num_procs)
  };

  uit::EnumeratedFunctor<Topology::node_id_t, uit::thread_id_t> thread_assigner{
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
