#include "conduit/ImplSpec.hpp"
#include "conduit/proc/ImsgDuct.hpp"

using ImplSel = uit::ImplSelector<
  uit::PendingDuct,
  uit::HeadTailDuct,
  uit::ImsgDuct
>;

#include "ProcDuct.hpp"
