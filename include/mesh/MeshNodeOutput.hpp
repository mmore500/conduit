#pragma once

#include <memory>
#include <stddef.h>

#include "conduit/Inlet.hpp"

namespace uit {

template<typename ImplSpec>
class MeshNodeOutput final : public uit::Inlet<ImplSpec> {

  size_t edge_id;

public:

  MeshNodeOutput(
    const uit::Inlet<ImplSpec> & inlet,
    const size_t edge_id_
  ) : uit::Inlet<ImplSpec>(inlet)
  , edge_id(edge_id_)
  { ; }

  size_t GetEdgeID() const { return edge_id; }

};

}
