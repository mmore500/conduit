#include "conduit/ImplSpec.hpp"
#include "conduit/thread/HeadTailDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::HeadTailDuct
>;

#include "ThreadDuct.hpp"
