#include "conduit/ImplSpec.h"
#include "conduit/proc/IrmsgDuct.h"

using ImplSel = uit::ImplSelector<
  uit::PendingDuct,
  uit::HeadTailDuct,
  uit::IrmsgDuct
>;

#include "ProcDuct.h"
