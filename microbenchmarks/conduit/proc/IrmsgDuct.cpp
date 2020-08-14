#include "conduit/ImplSpec.hpp"
#include "conduit/proc/IrmsgDuct.hpp"

using ImplSel = uit::ImplSelector<
  uit::PendingDuct,
  uit::HeadTailDuct,
  uit::IrmsgDuct
>;

#include "ProcDuct.hpp"
