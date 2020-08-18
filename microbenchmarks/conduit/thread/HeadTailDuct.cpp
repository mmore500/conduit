#include "conduit/ImplSpec.hpp"
#include "conduit/thread/HeadTailDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::PendingDuct,
  uit::HeadTailDuct
>;

#include "ThreadDuct.hpp"
