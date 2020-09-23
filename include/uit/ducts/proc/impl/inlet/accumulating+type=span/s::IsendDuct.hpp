#pragma once

#include "impl/AccumulatingSpanIsendDuct.hpp"

namespace uit {
namespace s {

template<typename ImplSpec>
class IsendDuct
: public internal::AccumulatingSpanIsendDuct<ImplSpec, false>
{

  using parent_t = internal::AccumulatingSpanIsendDuct<ImplSpec, false>;

public:

  // inherit parent's constructors
  using parent_t::parent_t;

};


} // namespace t
} // namespace uit
