#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/accumulating+type=fundamental/f::AtomicAccumulatingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::f::AtomicAccumulatingDuct
>;

#include "../ThreadDuct.hpp"
