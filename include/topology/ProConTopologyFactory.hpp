#pragma once

#include "Topology.hpp"
#include "TopoEdge.hpp"
#include "TopoNode.hpp"

namespace uit {

Topology make_producer_consumer_topology(const size_t cardinality) {

  Topology res;

  size_t edge_counter{};

  for (size_t pair = 0; pair < cardinality/2; ++pair) {

    uit::TopoEdge edge{edge_counter++};
    res.push_back(TopoNode{
      {},
      {edge.GetInlet()}
    });
    res.push_back(TopoNode{
      {edge.GetOutlet()},
      {}
    });

  }

  // for odd cardinality, add a loop pipe
  if (cardinality%2) {

    uit::TopoEdge self_edge{edge_counter++};
    res.push_back(TopoNode{
      {self_edge.GetOutlet()},
      {self_edge.GetInlet()}
    });

  };

  return res;

}

struct ProConTopologyFactory {
  Topology operator()(const size_t cardinality) const {
    return make_producer_consumer_topology(cardinality);
  }
  static std::string GetName() { return "Producer-Consumer Topology"; }
};

}
