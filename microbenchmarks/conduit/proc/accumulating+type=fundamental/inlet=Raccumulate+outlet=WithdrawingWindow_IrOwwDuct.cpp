#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/accumulating+type=fundamental/inlet=Raccumulate+outlet=WithdrawingWindow_IrOwwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IrOwwDuct
>;

#include "../ProcDuct.hpp"
