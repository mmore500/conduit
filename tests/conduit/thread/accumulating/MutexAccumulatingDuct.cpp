#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/thread/accumulating/MutexAccumulatingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::MutexAccumulatingDuct,
  uit::MutexAccumulatingDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "AccumulatingThreadDuct.hpp"
