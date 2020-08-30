#include "conduit/ImplSpec.hpp"
#include "conduit/proc/IrmsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IrmsgDuct
>;

#include "ProcDuct.hpp"
