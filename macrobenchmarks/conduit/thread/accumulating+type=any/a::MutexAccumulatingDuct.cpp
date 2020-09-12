#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/accumulating+type=any/a::MutexAccumulatingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::MutexAccumulatingDuct,
  uit::a::MutexAccumulatingDuct
>;

#include "../ThreadDuct.hpp"
