#pragma once

#include <memory>
#include <stddef.h>

#include "../../uit/spouts/Outlet.hpp"

namespace netuit {

template<typename ImplSpec>
class MeshNodeInput final
: public ImplSpec::template outlet_wrapper_t< uit::Outlet<ImplSpec> > {

  using parent_t
    = typename ImplSpec::template outlet_wrapper_t<uit::Outlet<ImplSpec>>;

  size_t edge_id;

public:

  MeshNodeInput(
    const parent_t& outlet,
    const size_t edge_id_
  ) : parent_t(outlet)
  , edge_id(edge_id_)
  { ; }

  size_t GetEdgeID() const { return edge_id; }

};

} // namespace netuit
