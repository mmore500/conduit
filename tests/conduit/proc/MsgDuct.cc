#include "conduit/ImplSpec.h"
#include "conduit/proc/MsgDuct.h"

using ImplSel = uit::ImplSelector<
  uit::PendingDuct,
  uit::HeadTailDuct,
  uit::MsgDuct
>;

#include "ProcDuct.h"
