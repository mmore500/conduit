#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/accumulating/inlet=Accumulate+outlet=WithdrawingWindow_IaOwwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IaOwwDuct
>;

#include "../ProcDuct.hpp"
