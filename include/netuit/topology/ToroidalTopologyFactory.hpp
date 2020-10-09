#pragma once

#include <algorithm>
#include <cmath>
#include <list>
#include <numeric>
#include <tuple>
#include <unordered_map>

#include "../../../third-party/Empirical/source/base/vector.h"
#include "../../../third-party/Empirical/source/tools/tuple_utils.h"

#include "TopoEdge.hpp"
#include "Topology.hpp"
#include "TopoNode.hpp"

#include "uitsl/debug/mapping_utils.hpp"
#include "uitsl/math/math_utils.hpp"

namespace uit {

// TODO: MOVE TO ITS OWN FILE
template <typename T>
class UIDMap {
  using node_id_t = size_t;
  using node_tuple = std::tuple<bool, T, T>;

  size_t counter{};

  std::unordered_multimap<
    node_tuple,
    size_t,
    emp::TupleHash<bool, T, T>
  > map;

public:
  size_t operator[](const node_tuple& a) {
    const auto& [is_output, from_node, to_node] = a;
    const node_tuple complement{
      !is_output,
      from_node,
      to_node
    };

    if (map.count(complement)) {
      const auto it = map.find(complement);
      const auto [key, val] = *it;
      map.erase(it);
      return val;
    } else {
      return map.insert({a, counter++})->second;
    }
  }
};

Topology make_toroidal_topology(const Dims& dim_cardinality) {
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

  emp::vector<TopoNode> nodes(cardinality);
  uit::UIDMap<size_t> node_edge_map;

  auto get_neighbor = [&dim_cardinality](Point p, const size_t dim, const int n) -> Point {
    p[dim] = uit::circular_index(p[dim], dim_cardinality[dim], n);
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
    const auto n = uit::linear_decode(my_id, dim_cardinality);
    const auto neighbors = get_neighbors(n);
    for (const auto& neighbor : neighbors) {
      const auto neighbor_node = uit::linear_encode(neighbor, dim_cardinality);

      it->AddInput(node_edge_map[{false, neighbor_node, my_id}]);
      it->AddOutput(node_edge_map[{true, my_id, neighbor_node}]);
    }
  }
  return nodes;
}

struct ToroidalTopologyFactory {
  netuit::Topology operator()(const Dims& dim_cardinality) const {
    return make_toroidal_topology(dim_cardinality);
  }

  static std::string GetName() { return "Toroidal Topology"; }
  std::string GetSimpleName() const { return "toroidal"; }
};
};
