#pragma once

#include <unordered_map>
#include <set>
#include <stddef.h>

#include "mpi.h"

#include "../utility/assign_utils.h"
#include "../conduit/Conduit.h"
#include "../conduit/Duct.h"

#include "../distributed/mpi_utils.h"
#include "../topology/Topology.h"

#include "MeshNode.h"

namespace uit {
namespace internal {

template<typename ImplSpec>
class MeshTopology {

  using node_id_t = size_t;
  using edge_id_t = size_t;
  using node_t = MeshNode<ImplSpec>;
  using node_lookup_t = std::unordered_map<node_id_t, node_t>;

  // node_id -> node
  node_lookup_t nodes;

  // ordered by edge_id
  std::set<edge_id_t> edge_registry;
  // edge_id -> node_id
  std::unordered_map<edge_id_t, node_id_t> input_registry;
  std::unordered_map<edge_id_t, node_id_t> output_registry;

  void InitializeRegistries(const uit::Topology& topology) {
    for (node_id_t node_id = 0; node_id < topology.size(); ++node_id) {
      const uit::TopoNode& topo_node = topology[node_id];
      RegisterNodeInputs(node_id, topo_node);
      RegisterNodeOutputs(node_id, topo_node);
    }
  }

  void RegisterNodeInputs(
    const node_id_t node_id, const uit::TopoNode& topo_node
  ) {
    for (const uit::TopoNodeInput & input : topo_node.GetInputs()) {
      emp_assert(input_registry.count(input.GetEdgeID()) == 0);
      edge_registry.insert(input.GetEdgeID());
      input_registry[input.GetEdgeID()] = node_id;
    }
  }

  void RegisterNodeOutputs(
    const node_id_t node_id, const uit::TopoNode& topo_node
  ) {
    for (const uit::TopoNodeOutput& output : topo_node.GetOutputs()) {
      emp_assert(output_registry.count(output.GetEdgeID()) == 0);
      edge_registry.insert(output.GetEdgeID());
      output_registry[output.GetEdgeID()] = node_id;
    }
  }

  void InitializeNodes(
    const uit::Topology& topology,
    const std::function<proc_id_t(node_id_t)> proc_assignment,
    const MPI_Comm comm
  ) {

    // ensures that we include relevant nodes that don't have any edges
    for (node_id_t node_id = 0; node_id < topology.size(); ++node_id) {
      if (proc_assignment(node_id) == uit::get_proc_id(comm)) {
        InitializeNode(node_id);
      }
    }

  }

  void InitializeNode(const node_id_t node_id){
    if (nodes.count(node_id) == 0) nodes.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(node_id),
      std::forward_as_tuple(node_id)
    );
  }

  void InitializeEdges(
    const uit::Topology& topology,
    const std::function<proc_id_t(node_id_t)> proc_assignment,
    const MPI_Comm comm
  ) {
    for (edge_id_t edge : edge_registry) {
      const node_id_t input_id = input_registry.at(edge);
      const node_id_t output_id = output_registry.at(edge);
      // only construct infrastructure relevant to this proc
      // (but do need nodes that are connected to nodes on this proc)
      if (
        proc_assignment(input_id) == uit::get_proc_id(comm)
        || proc_assignment(output_id) == uit::get_proc_id(comm)
      ) {
        uit::Conduit<ImplSpec> conduit;

        InitializeNode(input_id);
        nodes.at(input_id).AddInput(
          MeshNodeInput<ImplSpec>{conduit.GetOutlet(), edge}
        );

        InitializeNode(output_id);
        nodes.at(output_id).AddOutput(
          MeshNodeOutput<ImplSpec>{conduit.GetInlet(), edge}
        );
      }
    }
  }

public:

  using value_type = typename node_lookup_t::value_type;

  MeshTopology(
    const uit::Topology & topology,
    const std::function<proc_id_t(node_id_t)> proc_assignment
      =uit::AssignIntegrated<proc_id_t>{},
    const MPI_Comm comm=MPI_COMM_WORLD
  ) {
    InitializeRegistries(topology);
    InitializeNodes(topology, proc_assignment, comm);
    InitializeEdges(topology, proc_assignment, comm);

    // ensure that input, output registries have same keys as edge registry
    emp_assert(
      edge_registry == [this](){
        std::set<edge_id_t> res;
        std::transform(
          std::begin(input_registry),
          std::end(input_registry),
          std::inserter(res, std::begin(res)),
          [](const auto & kv){ return kv.first; }
        );
        return res;
      }()
    );
    emp_assert(
      edge_registry == [this](){
        std::set<edge_id_t> res;
        std::transform(
          std::begin(output_registry),
          std::end(output_registry),
          std::inserter(res, std::begin(res)),
          [](const auto & kv){ return kv.first; }
        );
        return res;
      }()
    );

  }

  size_t GetNodeCount() const { return nodes.size(); }

  size_t GetEdgeCount() const { return edge_registry.size(); }

  typename node_lookup_t::iterator begin() { return std::begin(nodes); }

  typename node_lookup_t::iterator end() { return std::end(nodes); }

  typename node_lookup_t::const_iterator begin() const {
    return std::begin(nodes);
  }

  typename node_lookup_t::const_iterator end() const {
    return std::end(nodes);
  }

  const std::set<edge_id_t>& GetEdgeRegistry() const { return edge_registry; }

  const std::unordered_map<edge_id_t, node_id_t>& GetInputRegistry() const {
    return input_registry;
  }

  const std::unordered_map<edge_id_t, node_id_t>& GetOutputRegistry() const {
    return output_registry;
  }


  std::string ToString() const {
    std::stringstream ss;
    ss << "TODO" << std::endl;
    return ss.str();
  }

};

}
}
