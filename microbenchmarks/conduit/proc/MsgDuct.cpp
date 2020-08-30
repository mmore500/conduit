#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/MsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::MsgDuct
>;

#include "ProcDuct.hpp"
