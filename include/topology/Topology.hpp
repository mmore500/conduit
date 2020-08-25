#pragma once

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

#include "../utility/string_utils.hpp"
#include "TopoNode.hpp"

namespace uit {

class Topology {
    void InitializeRegistries(const uit::Topology& topology) {
        for (node_id_t node_id = 0; node_id < topology.size(); ++node_id) {
            const uit::TopoNode& topo_node = topology[node_id];
            RegisterNodeInputs(node_id, topo_node);
            RegisterNodeOutputs(node_id, topo_node);
        }
  // todo:: public?
  using node_id_t = size_t;
  using edge_id_t = size_t;
  using topology_t = emp::vector<TopoNode>;

  topology_t topology;

  // maps of edge ids to node ids
  std::unordered_map<edge_id_t, node_id_t> input_registry;
  std::unordered_map<edge_id_t, node_id_t> output_registry;
    }

    void RegisterNodeInputs(const node_id_t node_id, const uit::TopoNode& topo_node) {
        for (const uit::TopoNodeInput& input : topo_node.GetInputs()) {
            emp_assert(input_registry.count(input.GetEdgeID()) == 0);
            edge_registry.insert(input.GetEdgeID());
            input_registry[input.GetEdgeID()] = node_id;
        }
    }

    void RegisterNodeOutputs(const node_id_t node_id, const uit::TopoNode& topo_node) {
        for (const uit::TopoNodeOutput& output : topo_node.GetOutputs()) {
            emp_assert(output_registry.count(output.GetEdgeID()) == 0);
            edge_registry.insert(output.GetEdgeID());
            output_registry[output.GetEdgeID()] = node_id;
        }
    }

public:
    Topology() = default;
    Topology(const size_t size) : topology(size) { ; }
    Topology(const std::istream& is) {
        for ()
     }

    emp::vector<TopoNode>::const_iterator begin() const noexcept { return topology.begin(); }
    emp::vector<TopoNode>::const_iterator end() const noexcept { return topology.end(); }

    emp::vector<TopoNode>::const_iterator cbegin() const noexcept { return topology.cbegin(); }
    emp::vector<TopoNode>::const_iterator cend() const noexcept { return topology.cend(); }

    void push_back(const TopoNode& node) { topology.push_back(node); }
    void push_back(TopoNode&& node) { topology.push_back(std::move(node)); }

    template <typename... Args>
    void emplace_back(Args&&... args) { topology.emplace_back(args); }

    size_t GetSize() const noexcept { return topology.size(); }

    const TopoNode& operator[](size_t n) const { return topology[n]; }

    void print(const Topology& topo, std::ostream& os = std::cout) const noexcept {
        for (size_t i = 0; i < topo.size(); ++i) {
            os << i << " " << topo[i] << std::endl;
        }
    }

    std::string ToString() const noexcept {
        std::ostringstream oss;
        print(*this, oss);
        return oss.str();
    }
};

}
