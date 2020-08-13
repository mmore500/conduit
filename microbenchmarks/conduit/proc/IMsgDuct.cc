#include "conduit/ImplSpec.hpp"
#include "conduit/proc/IMsgDuct.hpp"

using ImplSel = uit::ImplSelector<
  uit::PendingDuct,
  uit::HeadTailDuct,
  uit::IMsgDuct
>;

#include "ProcDuct.hpp"
