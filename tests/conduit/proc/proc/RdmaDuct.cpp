#include "conduit/ImplSpec.hpp"
#include "conduit/proc/RdmaDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::HeadTailDuct,
  uit::RdmaDuct
>;

#include "ProcDuct.hpp"
