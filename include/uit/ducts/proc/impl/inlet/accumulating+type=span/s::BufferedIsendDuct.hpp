#pragma once

#include "impl/AccumulatingSpanIsendDuct.hpp"

namespace uit {
namespace s {

template<typename ImplSpec>
class BufferedIsendDuct
: public internal::AccumulatingSpanIsendDuct<ImplSpec, true>
{

  using parent_t = internal::AccumulatingSpanIsendDuct<ImplSpec, true>;

public:

  // inherit parent's constructors
  using parent_t::parent_t;

};


} // namespace t
} // namespace uit
