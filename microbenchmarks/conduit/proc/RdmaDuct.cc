#include "conduit/ImplSpec.h"
#include "conduit/proc/RdmaDuct.h"

using ImplSel = uit::ImplSelector<
  uit::PendingDuct,
  uit::HeadTailDuct,
  uit::RdmaDuct
>;

#include "ProcDuct.h"
