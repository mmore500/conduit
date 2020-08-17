#include "conduit/ImplSpec.hpp"
#include "conduit/proc/RmsgDuct.hpp"

using ImplSel = uit::ImplSelector<
  uit::PendingDuct,
  uit::HeadTailDuct,
  uit::RmsgDuct
>;

#include "ProcDuct.hpp"
