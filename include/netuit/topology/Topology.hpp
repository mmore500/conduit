#pragma once
#ifndef NETUIT_TOPOLOGY_TOPOLOGY_HPP_INCLUDE
#define NETUIT_TOPOLOGY_TOPOLOGY_HPP_INCLUDE

#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>

#ifndef __EMSCRIPTEN__
#include <metis.h>
#endif

#include "../../../third-party/Empirical/source/tools/hash_utils.h"
#include "../../../third-party/Empirical/source/tools/keyname_utils.h"

#include "../../uitsl/debug/EnumeratedFunctor.hpp"
#include "../../uitsl/debug/metis_utils.hpp"
#include "../../uitsl/polyfill/identity.hpp"
#include "../../uitsl/utility/stream_utils.hpp"

#include "TopoNode.hpp"

namespace netuit {

class Topology {
public:
  using node_id_t = size_t;
  using edge_id_t = size_t;
private:
  using topology_t = emp::vector<TopoNode>;
  topology_t topology;

  // unordered_maps of edge ids to node ids
  std::unordered_map<edge_id_t, node_id_t> input_registry;
  std::unordered_map<edge_id_t, node_id_t> output_registry;

  // map of index to node_id
  std::function<node_id_t(node_id_t)> index_map{std::identity};

  /// Register node in Topology.
  /// Note: this does not put the node in the Topology.
  /// @param[in] node_id ID of node to register.
  /// @param[in] topo_node Node to register.
  void RegisterNode(const node_id_t node_id, const netuit::TopoNode& topo_node) {
    RegisterNodeInputs(node_id, topo_node);
    RegisterNodeOutputs(node_id, topo_node);
  }

  /// Register inputs of a node in Topology
  /// @param[in] node_id ID of node to register.
  /// @param[in] topo_node Node to register.
  void RegisterNodeInputs(const node_id_t node_id, const netuit::TopoNode& topo_node) {
    for (const auto& input : topo_node.GetInputs()) {
      emp_assert(input_registry.count(input.GetEdgeID()) == 0);
      input_registry[input.GetEdgeID()] = node_id;
    }
  }

  /// Register outputs of a node in Topology
  /// @param[in] node_id ID of node to register.
  /// @param[in] topo_node Node to register.
  void RegisterNodeOutputs(const node_id_t node_id, const netuit::TopoNode& topo_node) {
    for (const auto& output : topo_node.GetOutputs()) {
      emp_assert(output_registry.count(output.GetEdgeID()) == 0);
      output_registry[output.GetEdgeID()] = node_id;
    }
  }

  /// Return outputs of a node in Topology
  /// @param[in] topo_node Node to return outputs of.
  /// @return Vector of node IDs which topo_node is an output of.
  emp::vector<node_id_t> GetNodeOutputs(const netuit::TopoNode& node) const {
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
    uitsl::read_lines(is, std::back_inserter(lines));

    // map of node ids to nodes
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
    // make sure the node ids are less than the number of lines
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

  /// Insert node in Topology.
  /// @param[in] node Node to insert.
  void push_back(const TopoNode& node) {
    const size_t id = topology.size();
    topology.push_back(node);
    RegisterNode(id, node);
  }

  /// Insert node in Topology.
  /// @param[in] node Node to insert.
  void push_back(TopoNode&& node) {
    const size_t id = topology.size();
    RegisterNode(id, node);
    topology.push_back(std::move(node));
  }

  /// Emplace node in Topology.
  /// @param[in] args Arguments of node to emplace.
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

  /// Sets map of node IDs to Canonical node IDs.
  /// @param[in] map Map of node IDs to Canonical node IDs
  void SetMap(const std::unordered_map<node_id_t, node_id_t>& map) {
    index_map = uitsl::EnumeratedFunctor<node_id_t, node_id_t>(map);
  }

  /// A Canonical node ID is the ID of the node in the original subtopology.
  /// After GetSubTopology is called, nodes of that topology get a new ID, but
  /// their Canonical ID stays the same. This provides a way to trace back nodes
  /// to the original, parent topology.
  /// @param node_id[in] ID of node to get Canonical ID of.
  /// @return Canonical ID of node.
  node_id_t GetCanonicalNodeID(const node_id_t node_id) const {
    return index_map(node_id);
  }

  /// Return Compressed Sparse Row (CSR) representation of topology.
  /// For more info, see: https://en.wikipedia.org/wiki/Sparse_matrix#Compressed_sparse_row_(CSR,_CRS_or_Yale_format)
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
  emp::vector<size_t> Optimize(const int32_t parts) const {

    // set up result vector
    size_t nodes = topology.size();
    emp::vector<size_t> result(nodes);

    #ifndef __EMSCRIPTEN__
    // use default options
    size_t options[METIS_NOPTIONS];
    METIS_SetDefaultOptions(options);

    // set up variables
    size_t n_cons = 1;
    size_t volume;

    // get topology as CSR
    auto [xadj, adjacency] = AsCSR();

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
      &volume, // edge-cut or total comm volume of the solution
      result.data() // partition vector of the graph
    );

    uitsl::metis::verify(status);
    #endif

    return result;
  }

  /// Outputs this Topology's Edge list. This format stores node IDs with
  /// their respective edge IDs.
  /// For more info, see https://networkx.github.io/documentation/stable/reference/readwrite/edgelist.html
  /// @param[out] os Stream to output Edge list to.
  void PrintEdgeList(std::ostream& os = std::cout) const noexcept {
    for (size_t i = 0; i < topology.size(); ++i) {
      for (const auto& output : topology[i].GetOutputs()) {
        os << i << " " << output << std::endl;
      }
    }
  }

  /// Outputs this Topology's Adjacency list. This format stores node IDs with
  /// the IDs of nodes they are connected to.
  /// For more info, see https://networkx.github.io/documentation/stable/reference/readwrite/adjlist.html
  /// @param[out] os Stream to output Adjacency list to.
  void PrintAdjacencyList(std::ostream& os = std::cout) const noexcept {
    for (size_t i = 0; i < topology.size(); ++i) {
      os << i;
      for (const auto& output : GetNodeOutputs(topology[i])) {
        os << " " << output;
      }
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

  /// Returns a subtopology made up of all the nodes passed onto it,
  /// based on the topology it was called on.
  /// @param[in] node_ids Node IDs of nodes to make subtopology out of.
  /// @param[in] translator Map of Node IDs to Canonical Node IDs.
  /// @return Subtopology from Node IDs.
  Topology GetSubTopology(const std::unordered_set<size_t>& node_ids) const {
    emp::vector<netuit::TopoNode> nodes;
    std::unordered_map<node_id_t, node_id_t> translator;

    // fill subtopology with all nodes in node_ids
    for (const size_t i : node_ids) {
      nodes.push_back(topology[i]);
      translator[translator.size()] = i;
    }

    // fix subtopology to exclude external nodes
    for (auto& node : nodes) {
      for (const auto& output : node.GetOutputs()) {
        if (!node_ids.count(
          output_registry.at(output.GetEdgeID())
        )) node.RemoveOutput(output);
      }
      for (const auto& input : node.GetInputs()) {
        if (node_ids.count(
          input_registry.at(input.GetEdgeID())
        )) node.RemoveInput(input);
      }
    }
    Topology subtopo(nodes);
    subtopo.SetMap(translator);
    return subtopo;
  }

  /// Returns this Topology's adjacency list as a string.
  /// @return String describing topology
  std::string ToString() const noexcept {
    std::ostringstream oss;
    PrintAdjacencyList(oss);
    return oss.str();
  }
};

} // namespace netuit

#endif // #ifndef NETUIT_TOPOLOGY_TOPOLOGY_HPP_INCLUDE
