#pragma once

#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>

#include "../../third-party/Empirical/source/tools/hash_utils.h"
#include "../../third-party/Empirical/source/tools/keyname_utils.h"

extern "C" {
#include <metis.h>
}
#include "../utility/EnumeratedFunctor.hpp"
#include "../utility/identity.hpp"
#include "../utility/stream_utils.hpp"

#include "TopoNode.hpp"

namespace uit {

class Topology {
public:
  using node_id_t = size_t;
  using edge_id_t = size_t;
private:
  // todo: change this to an unordered_map
  using topology_t = emp::vector<TopoNode>;
  topology_t topology;

  // unordered_maps of edge ids to node ids
  std::unordered_map<edge_id_t, node_id_t> input_registry;
  std::unordered_map<edge_id_t, node_id_t> output_registry;

  // map of index to node_id
  std::function<node_id_t(node_id_t)> index_map{uit::identity};

  void RegisterNode(const node_id_t node_id, const uit::TopoNode& topo_node) {
    RegisterNodeInputs(node_id, topo_node);
    RegisterNodeOutputs(node_id, topo_node);
  }

  void RegisterNodeInputs(const node_id_t node_id, const uit::TopoNode& topo_node) {
    for (const auto& input : topo_node.GetInputs()) {
      emp_assert(input_registry.count(input.GetEdgeID()) == 0);
      input_registry[input.GetEdgeID()] = node_id;
    }
  }

  void RegisterNodeOutputs(const node_id_t node_id, const uit::TopoNode& topo_node) {
    for (const auto& output : topo_node.GetOutputs()) {
      emp_assert(output_registry.count(output.GetEdgeID()) == 0);
      output_registry[output.GetEdgeID()] = node_id;
    }
  }

  emp::vector<node_id_t> GetNodeOutputs(const uit::TopoNode& node) const {
    emp::vector<node_id_t> res;
    for (const auto& edge : node.GetOutputs()) {
      res.push_back(input_registry.at(edge.GetEdgeID()));
    }
    return res;
  }

public:
  using value_type = TopoNode;

  Topology() = default;

  template <typename... Args>
  Topology(Args&&... args) : topology(std::forward<Args>(args)...) {
    for (size_t i = 0; i < topology.size(); ++i) {
      RegisterNode(i, topology[i]);
    }
  }

  Topology(std::istream& is) {
    emp::vector<std::string> lines;
    // read file lines into vector
    uit::read_lines(is, std::back_inserter(lines));

    // map of node itds to nodes
    std::map<node_id_t, TopoNode> node_map;
    // put nodes into map
    for (const std::string& line : lines) {
      std::istringstream iss(line);
      node_id_t node_id;
      iss >> node_id;
      iss >> node_map[node_id];
    }
    // make sure we inserted every line
    emp_assert(lines.size() == node_map.size());
    // make sure the node ids are less than the number of line
    emp_assert( std::all_of(
      std::begin(node_map),
      std::end(node_map),
      [num_lines = lines.size()](const auto& kv) {
        const auto& [id, node] = kv;
        return id < num_lines;
      }
    ) );
    // insert all nodes into topology
    std::for_each(
      std::begin(node_map),
      std::end(node_map),
      [this](const auto& kv) {
        const auto& [id, node] = kv;
        push_back(node);
      }
    );
  }
  // topology iterators must only be const
  topology_t::const_iterator begin() const noexcept { return topology.begin(); }
  topology_t::const_iterator end() const noexcept { return topology.end(); }

  topology_t::const_iterator cbegin() const noexcept { return topology.cbegin(); }
  topology_t::const_iterator cend() const noexcept { return topology.cend(); }

  void push_back(const TopoNode& node) {
    const size_t id = topology.size();
    topology.push_back(node);
    RegisterNode(id, node);
  }
  void push_back(TopoNode&& node) {
    const size_t id = topology.size();
    topology.push_back(std::move(node));
    RegisterNode(id, node);
  }

  template <typename... Args>
  void emplace_back(Args&&... args) {
    const size_t id = topology.size();
    topology.emplace_back(std::forward(args)...);
    RegisterNode(id, topology[id]);
  }
  /// Returns number of nodes in topology
  /// @return number of nodes in topology
  size_t GetSize() const noexcept { return topology.size(); }

  /// Return nth node in topology
  /// @return const ref to nth node
  const TopoNode& operator[](size_t n) const { return topology[n]; }

  void SetMap(const std::unordered_map<node_id_t, node_id_t>& map) {
    index_map = uit::EnumeratedFunctor<node_id_t, node_id_t>(map);
  }

  node_id_t GetCanonicalNodeID(const node_id_t node_id) const {
    return index_map(node_id);
  }

  /// Return Compressed Sparse Row (CSR) representation of topology
  /// @return std::pair of vectors of int
  auto AsCSR() const {
    // get vector with degree of each node
    emp::vector<int> degrees;
    std::transform(
      std::begin(topology),
      std::end(topology),
      std::back_inserter(degrees),
      [](const auto& node){ return node.GetNumOutputs(); }
    );
    // get each starting position of each node's adjacency list
    emp::vector<int> x_adj{0};
    std::partial_sum(
      std::begin(degrees),
      std::end(degrees),
      std::back_inserter(x_adj)
    );
    // build vector of concatenated adjacency lists
    emp::vector<int> adjacency;
    std::for_each(
      std::begin(topology),
      std::end(topology),
      [this, &adjacency](const auto& node){
        const auto outputs = GetNodeOutputs(node);
        adjacency.insert(
          std::end(adjacency),
          std::begin(outputs),
          std::end(outputs)
        );
      }
    );  return std::make_pair(x_adj, adjacency);
  }

  /// Apply METIS' K-way partitioning algorithm to subdivide topology
  /// @param parts number of parts to subdivide topology into
  /// @return vector indicating what partition each vertex should go into
  emp::vector<idx_t> Optimize(int32_t parts) const {
    // set up variables
    idx_t nodes = topology.size();
    idx_t n_cons = 1;
    idx_t volume;

    // get topology as CSR
    auto [xadj, adjacency] = AsCSR();

    // set up result vector
    emp::vector<idx_t> result(nodes);

    // set up options array
    // in this case, it's just the default options
    idx_t options[METIS_NOPTIONS];
    METIS_SetDefaultOptions(options);

    // call partitioning algorithm
    auto status = METIS_PartGraphKway(
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
      &volume, // edge-cut or total comm volume of the solution
      result.data() // partition vector of the graph
    );

    // deal with return code
    switch(status) {
      case METIS_OK:
        break;
      case METIS_ERROR_INPUT:
        throw std::invalid_argument("Error in input.");
        break;
      case METIS_ERROR_MEMORY:
        throw std::bad_alloc("Out of memory!!!");
        break;
      case METIS_ERROR:
        throw std::runtime_error("Unspecified error.");
        break;
    }
  }

  void PrintEdgeList(std::ostream& os = std::cout) const noexcept {
    // FIX THIS
    // node1 node2
    // node1 node3
    // etc
    for (size_t i = 0; i < topology.size(); ++i) {
      for (const auto& output : topology[i].GetOutputs()) {
        os << i << " " << output << std::endl;
      }
    }
  }

  void PrintAdjacencyList(std::ostream& os = std::cout) const noexcept {
    // CHANGE NAME
    for (size_t i = 0; i < topology.size(); ++i) {
      os << i << " " << GetNodeOutputs(topology[i]);
      os << std::endl;
    }
  }

  void PrintPartition(const emp::vector<int>& partition, std::ostream& os = std::cout) const {
    auto name_node = [&partition] (const size_t index) -> std::string {
      return emp::keyname::pack({
        {"node_id", emp::to_string(index)},
        {"partition", emp::to_string(partition[index])}
      });
    };

    for (size_t i = 0; i < topology.size(); ++i) {
      os << name_node(i) << " ";
      os << topology[i].GetNumOutputs() << std::endl;

      for (const auto& node : GetNodeOutputs(topology[i])) {
        os << name_node(node) << std::endl;
      }
    }
  }

  std::string ToString() const noexcept {
    std::ostringstream oss;
    PrintNodes(oss);
    return oss.str();
  }
};

}
