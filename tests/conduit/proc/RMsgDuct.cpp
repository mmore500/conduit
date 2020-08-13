#include "conduit/ImplSpec.hpp"
#include "conduit/proc/RMsgDuct.hpp"

using ImplSel = uit::ImplSelector<
  uit::PendingDuct,
  uit::HeadTailDuct,
  uit::RMsgDuct
>;

#include "ProcDuct.hpp"
