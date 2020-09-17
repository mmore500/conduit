#pragma once

#include <stddef.h>

#include "../../uitsl/utility/print_utils.hpp"

namespace netuit {

class TopoNodeInput {

  size_t edge_id;

public:

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

};

} // namespace netuit
