#include "conduit/ImplSpec.h"
#include "conduit/thread/HeadTailDuct.h"

using ImplSel = uit::ImplSelector<
  uit::PendingDuct,
  uit::HeadTailDuct
>;

#include "ThreadDuct.h"
