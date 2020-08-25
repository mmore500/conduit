#pragma once

#include <algorithm>
#include <cmath>
#include <list>
#include <numeric>

#include "TopoEdge.hpp"
#include "Topology.hpp"
#include "TopoNode.hpp"

#include "../../third-party/Empirical/source/tools/tuple_utils.h"

#include "../utility/mapping_utils.hpp"
#include "../utility/math_utils.hpp"

namespace uit {

template <typename T>
class UIDMap {
  using node_id_t = size_t;
  using node_pair = std::tuple<T, T>;

  std::unordered_map<
    node_pair,
    size_t,
    emp::TupleHash<T, T>
  > map;

public:

  size_t operator[](const node_pair& a) {
    if (!map.count(a)) map[a] = map.size();
    return map.at(a);
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
  auto get_neighbors = [&get_neighbor, &dim_cardinality](const Point& p) {
    emp::vector<Point> neighbors;

    for (size_t i = 0; i < p.size(); ++i) {
      neighbors.push_back(get_neighbor(p, i, +1));
      neighbors.push_back(get_neighbor(p, i, -1));
    }

    return neighbors;
  };

  for (auto it = res.begin(); it != res.end(); ++it) {
    const auto n = uit::linear_decode(it - res.begin(), dim_cardinality);
    const auto neighbors = get_neighbors(n);

    for (const auto& neighbor : neighbors) {
      it->AddInput(uit::linear_encode(neighbor, dim_cardinality));
    }
  }

  return res;
}

struct ToroidalTopologyFactory {
  uit::Topology operator()(const Dims& dim_cardinality) const {
    return make_toroidal_topology(dim_cardinality);
  }
  static std::string GetName() { return "Toroidal Topology"; }
};
};
