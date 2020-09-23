#pragma once

#include "impl/AccumulatingTrivialIsendDuct.hpp"

namespace uit {
namespace t {

template<typename ImplSpec>
class IsendDuct
: public internal::AccumulatingTrivialIsendDuct<ImplSpec, false>
{

  using parent_t = internal::AccumulatingTrivialIsendDuct<ImplSpec, false>;

public:

  // inherit parent's constructors
  using parent_t::parent_t;

};


} // namespace t
} // namespace uit
