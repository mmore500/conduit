#pragma once

#include "impl/AccumulatingTrivialIsendDuct.hpp"

namespace uit {
namespace t {

template<typename ImplSpec>
class BufferedIsendDuct
: public internal::AccumulatingTrivialIsendDuct<ImplSpec, true>
{

  using parent_t = internal::AccumulatingTrivialIsendDuct<ImplSpec, true>;

public:

  // inherit parent's constructors
  using parent_t::parent_t;

};


} // namespace t
} // namespace uit
