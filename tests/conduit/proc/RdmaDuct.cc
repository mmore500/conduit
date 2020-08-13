#include "conduit/ImplSpec.hpp"
#include "conduit/proc/RdmaDuct.hpp"

using ImplSel = uit::ImplSelector<
  uit::PendingDuct,
  uit::HeadTailDuct,
  uit::RdmaDuct
>;

#include "ProcDuct.hpp"
