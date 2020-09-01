#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/accumulating/MutexAccumulatingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::MutexAccumulatingDuct
>;

#include "../ThreadDuct.hpp"
