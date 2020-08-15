#pragma once

#include <algorithm>
#include <cmath>
#include <list>
#include <numeric>

#include "TopoEdge.hpp"
#include "Topology.hpp"
#include "TopoNode.hpp"

#include "../utility/mapping_utils.hpp"
#include "../utility/math_utils.hpp"

namespace uit {
Topology make_toroidal_topology(const Dims& dim_cardinality) {
  /*
  * goal
  * make toroidal topology (such that opposite edges are the same edge)
  */
  size_t edge_counter{};
  const size_t dims = dim_cardinality.size();
  const size_t cardinality = std::accumulate(
    dim_cardinality.begin(),
    dim_cardinality.end(),
    0
  );

  Topology res(cardinality);

  auto get_neighbor = [&dim_cardinality](Point p, const size_t dim, const int n) -> Point {
    p[dim] = uit::circular_index(p[dim], dim_cardinality[dim], n);
    return p;
  };

  // returns a vector containing all neighbors
  auto get_neighbors = [&get_neighbor, &dim_cardinality](const Point& p) {
    emp::vector<Point> neighbors;

    for (int i = 0; i < p.size(); ++i) {
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
};
