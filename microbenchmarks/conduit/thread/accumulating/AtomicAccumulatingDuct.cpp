#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/accumulating/AtomicAccumulatingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::AtomicAccumulatingDuct
>;

#include "../ThreadDuct.hpp"
