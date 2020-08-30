#include "conduit/ImplSpec.hpp"
#include "conduit/proc/MsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::MsgDuct
>;

#include "ProcDuct.hpp"
