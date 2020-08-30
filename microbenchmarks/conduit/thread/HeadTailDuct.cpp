#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/thread/HeadTailDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::HeadTailDuct
>;

#include "ThreadDuct.hpp"
