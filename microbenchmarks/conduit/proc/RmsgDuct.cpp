#include "conduit/ImplSpec.hpp"
#include "conduit/proc/RmsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::HeadTailDuct,
  uit::RmsgDuct
>;

#include "ProcDuct.hpp"
