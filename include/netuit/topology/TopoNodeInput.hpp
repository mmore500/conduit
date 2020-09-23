#pragma once

#include <stddef.h>
#include <utility>
#include <iostream>

#include "../../uitsl/utility/print_utils.hpp"

namespace netuit {

class TopoNodeInput {

  size_t edge_id;

public:

  TopoNodeInput() = default;
  TopoNodeInput(const size_t edge_id_)
  : edge_id(edge_id_)
  { ; }

  size_t GetEdgeID() const { return edge_id; }

  std::string ToString() const {
    std::stringstream ss;
    ss << uitsl::format_member(
      "size_t edge_id",
      edge_id
    );
    return ss.str();
  }

  bool operator==(const TopoNodeInput& other) const {
    return edge_id == other.edge_id;
  }

  friend std::ostream& operator<<(std::ostream& os, const TopoNodeInput& node) {
    os << node.edge_id << " ";
    return os;
  }

};
} // namespace netuit

namespace std {
  template <>
  struct hash<uit::TopoNodeInput> {
    std::size_t operator()(const uit::TopoNodeInput& k) const {
      return std::hash<size_t>()(k.GetEdgeID());
    }
  };
} // namespace std
