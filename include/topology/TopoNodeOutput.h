#pragma once

#include <stddef.h>

namespace uit {

class TopoNodeOutput {

  size_t edge_id;

public:

  TopoNodeOutput(const size_t edge_id_)
  : edge_id(edge_id_)
  { ; }

  size_t GetEdgeID() const { return edge_id; }

};

}
