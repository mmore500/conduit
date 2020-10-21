#pragma once
#ifndef NETUIT_TOPOLOGY_COMPLETETOPOLOGYFACTORY_HPP_INCLUDE
#define NETUIT_TOPOLOGY_COMPLETETOPOLOGYFACTORY_HPP_INCLUDE

#include "../../../third-party/Empirical/source/base/vector.h"

#include "TopoEdge.hpp"
#include "Topology.hpp"
#include "TopoNode.hpp"

namespace netuit {

netuit::Topology make_complete_topology(const size_t cardinality) {

  emp::vector<netuit::TopoNode> res( cardinality );

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

  netuit::Topology operator()(const emp::vector<size_t> cardinality) const {
    emp_assert(cardinality.size() == 1);
    return make_complete_topology(cardinality.front());
  }
  static std::string GetName() { return "Complete Topology"; }

  static std::string GetSlug() const { return "complete"; }

};

} // namespace netuit

#endif // #ifndef NETUIT_TOPOLOGY_COMPLETETOPOLOGYFACTORY_HPP_INCLUDE
