#include "conduit/ImplSpec.hpp"
#include "conduit/proc/RdmaDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::PendingDuct,
  uit::HeadTailDuct,
  uit::RdmaDuct
>;

#include "ProcDuct.hpp"
