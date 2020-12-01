#pragma once
#ifndef NETUIT_ARRANGE_TOROIDALTOPOLOGYFACTORY_HPP_INCLUDE
#define NETUIT_ARRANGE_TOROIDALTOPOLOGYFACTORY_HPP_INCLUDE

#include <algorithm>
#include <cmath>
#include <list>
#include <numeric>
#include <tuple>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/datastructs/tuple_utils.hpp"

#include "../../uitsl/math/mapping_utils.hpp"
#include "../../uitsl/math/math_utils.hpp"
#include "../../uitsl/utility/UIDMap.hpp"

#include "../topology/TopoEdge.hpp"
#include "../topology/Topology.hpp"
#include "../topology/TopoNode.hpp"

namespace netuit {

netuit::Topology make_toroidal_topology(const Dims& dim_cardinality) {
  /*
  * goal
  * make toroidal topology (such that opposite edges are the same edge)
  */
  const size_t cardinality = std::accumulate(
    dim_cardinality.begin(),
    dim_cardinality.end(),
    1,
    std::multiplies<size_t>()
  );

  emp::vector<netuit::TopoNode> nodes(cardinality);
  uitsl::UIDMap<size_t> node_edge_map;

  auto get_neighbor = [&dim_cardinality](Point p, const size_t dim, const int n) -> Point {
    p[dim] = uitsl::circular_index(p[dim], dim_cardinality[dim], n);
    return p;
  };

  // returns a vector containing all neighbors
  auto get_neighbors = [&get_neighbor](const Point& p) {
    emp::vector<Point> neighbors;

    for (size_t i = 0; i < p.size(); ++i) {
      neighbors.push_back(get_neighbor(p, i, +1));
      neighbors.push_back(get_neighbor(p, i, -1));
    }

    return neighbors;
  };

  for (auto it = nodes.begin(); it != nodes.end(); ++it) {
    const size_t my_id = std::distance(nodes.begin(), it);
    const auto n = uitsl::linear_decode(my_id, dim_cardinality);
    const auto neighbors = get_neighbors(n);
    for (const auto& neighbor : neighbors) {
      const auto neighbor_node = uitsl::linear_encode(neighbor, dim_cardinality);

      it->AddInput(node_edge_map[{false, neighbor_node, my_id}]);
      it->AddOutput(node_edge_map[{true, my_id, neighbor_node}]);
    }
  }
  return nodes;
}

struct ToroidalTopologyFactory {

  netuit::Topology operator()(const size_t cardinality) const {
    return make_toroidal_topology({cardinality});
  }

  netuit::Topology operator()(const Dims& dim_cardinality) const {
    return make_toroidal_topology(dim_cardinality);
  }

  static std::string GetName() { return "Toroidal Topology"; }

  static std::string GetSlug() { return "toroidal"; }

};

} // namespace netuit

#endif // #ifndef NETUIT_ARRANGE_TOROIDALTOPOLOGYFACTORY_HPP_INCLUDE
