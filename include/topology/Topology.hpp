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
  // todo:: public?
  using node_id_t = size_t;
  using edge_id_t = size_t;
  using topology_t = emp::vector<TopoNode>;

  topology_t topology;

  // maps of edge ids to node ids
  std::unordered_map<edge_id_t, node_id_t> input_registry;
  std::unordered_map<edge_id_t, node_id_t> output_registry;

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







    }

    }
};

}
