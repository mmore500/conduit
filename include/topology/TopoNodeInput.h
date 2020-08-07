#pragma once

#include <stddef.h>

namespace uit {

class TopoNodeInput {

  size_t edge_id;

public:

  TopoNodeInput(const size_t edge_id_)
  : edge_id(edge_id_)
  { ; }

  size_t GetEdgeID() const { return edge_id; }

};

}
