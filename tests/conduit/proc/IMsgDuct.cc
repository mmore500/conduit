#include "conduit/ImplSpec.h"
#include "conduit/proc/IMsgDuct.h"

using ImplSel = uit::ImplSelector<
  uit::PendingDuct,
  uit::HeadTailDuct,
  uit::IMsgDuct
>;

#include "ProcDuct.h"
