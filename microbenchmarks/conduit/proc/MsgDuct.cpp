#include "conduit/ImplSpec.hpp"
#include "conduit/proc/MsgDuct.hpp"

using ImplSel = uit::ImplSelector<
  uit::PendingDuct,
  uit::HeadTailDuct,
  uit::MsgDuct
>;

#include "ProcDuct.hpp"
