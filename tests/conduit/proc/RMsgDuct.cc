#include "conduit/ImplSpec.h"
#include "conduit/proc/RMsgDuct.h"

using ImplSel = uit::ImplSelector<
  uit::PendingDuct,
  uit::HeadTailDuct,
  uit::RMsgDuct
>;

#include "ProcDuct.h"
