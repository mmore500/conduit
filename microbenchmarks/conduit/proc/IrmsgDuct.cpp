#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/IrmsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IrmsgDuct
>;

#include "ProcDuct.hpp"
