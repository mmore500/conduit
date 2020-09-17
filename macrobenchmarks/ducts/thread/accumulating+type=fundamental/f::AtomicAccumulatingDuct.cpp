#include "uit/setup/ImplSpec.hpp"
#include "uit/ducts/thread/accumulating+type=fundamental/f::AtomicAccumulatingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::f::AtomicAccumulatingDuct
>;

#include "../ThreadDuct.hpp"
