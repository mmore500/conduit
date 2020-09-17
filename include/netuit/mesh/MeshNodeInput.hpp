#pragma once

#include <memory>
#include <stddef.h>

#include "../../uit/spouts/Outlet.hpp"

namespace netuit {

template<typename ImplSpec>
class MeshNodeInput final : public uit::Outlet<ImplSpec> {

  size_t edge_id;

public:

  MeshNodeInput(
    const uit::Outlet<ImplSpec> & outlet,
    const size_t edge_id_
  ) : uit::Outlet<ImplSpec>(outlet)
  , edge_id(edge_id_)
  { ; }

  size_t GetEdgeID() const { return edge_id; }

};

} // namespace netuit
