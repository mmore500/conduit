#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/thread/accumulating/AtomicAccumulatingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::AtomicAccumulatingDuct,
  uit::AtomicAccumulatingDuct,
  uit::ThrowDuct
>;

#include "../ThreadDuct.hpp"
#include "AccumulatingThreadDuct.hpp"
