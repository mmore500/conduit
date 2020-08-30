#include "conduit/ImplSpec.hpp"
#include "conduit/proc/ImsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::ImsgDuct
>;

#include "ProcDuct.hpp"
