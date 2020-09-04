#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/accumulating/RaccumulateWithdrawingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::RaccumulateWithdrawingDuct
>;

#include "../ProcDuct.hpp"
