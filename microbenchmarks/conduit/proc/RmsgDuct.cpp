#include "conduit/ImplSpec.hpp"
#include "conduit/proc/RmsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::RmsgDuct
>;

#include "ProcDuct.hpp"
