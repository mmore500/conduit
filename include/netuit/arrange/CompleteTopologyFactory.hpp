#pragma once
#ifndef NETUIT_ARRANGE_COMPLETETOPOLOGYFACTORY_HPP_INCLUDE
#define NETUIT_ARRANGE_COMPLETETOPOLOGYFACTORY_HPP_INCLUDE

#include <cassert>
#include <vector>

#include "../topology/TopoEdge.hpp"
#include "../topology/Topology.hpp"
#include "../topology/TopoNode.hpp"

namespace netuit {

inline netuit::Topology make_complete_topology(const size_t cardinality) {

  std::vector<netuit::TopoNode> res( cardinality );

  size_t edge_counter{};

  for (size_t i{}; i < cardinality; ++i) {
    for (size_t j{}; j < cardinality; ++j) {
      if ( i != j ) { // exclude self-loops
        netuit::TopoEdge edge{edge_counter++};
        res[i].AddInput( edge.GetOutlet() );
        res[j].AddOutput( edge.GetInlet() );
      }
    }
  }

  return netuit::Topology{ res };

}

struct CompleteTopologyFactory {

  netuit::Topology operator()(const size_t cardinality) const {
    return make_complete_topology(cardinality);
  }

  netuit::Topology operator()(const std::vector<size_t> cardinality) const {
    assert(cardinality.size() == 1);
    return make_complete_topology(cardinality.front());
  }
  static std::string GetName() { return "Complete Topology"; }

  static std::string GetSlug() { return "complete"; }

};

} // namespace netuit

#endif // #ifndef NETUIT_ARRANGE_COMPLETETOPOLOGYFACTORY_HPP_INCLUDE
