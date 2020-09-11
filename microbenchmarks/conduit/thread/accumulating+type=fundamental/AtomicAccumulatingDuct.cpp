#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/accumulating+type=fundamental/AtomicAccumulatingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::AtomicAccumulatingDuct
>;

#include "../ThreadDuct.hpp"
