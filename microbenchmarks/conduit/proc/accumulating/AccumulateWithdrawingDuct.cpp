#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/accumulating/AccumulateWithdrawingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::AccumulateWithdrawingDuct
>;

#include "../ProcDuct.hpp"
