#pragma once

#include <algorithm>

#include "TopoEdge.hpp"
#include "Topology.hpp"
#include "TopoNode.hpp"

namespace uit {

uit::Topology make_loop_topology(const size_t cardinality) {

  uit::Topology res;

  size_t edge_counter{};

  std::generate_n(
    std::back_inserter(res),
    cardinality,
    [&](){
      uit::TopoEdge self_edge{edge_counter++};
      return uit::TopoNode{
        {self_edge.GetOutlet()},
        {self_edge.GetInlet()}
      };
    }
  );

  return res;

}

struct LoopTopologyFactory {
  uit::Topology operator()(const size_t cardinality) const {
    return make_loop_topology(cardinality);
  }
  static std::string GetName() { return "Loop Topology"; }
};

}
