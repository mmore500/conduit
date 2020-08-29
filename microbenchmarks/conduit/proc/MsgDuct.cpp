#include "conduit/ImplSpec.hpp"
#include "conduit/proc/MsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::HeadTailDuct,
  uit::MsgDuct
>;

#include "ProcDuct.hpp"
