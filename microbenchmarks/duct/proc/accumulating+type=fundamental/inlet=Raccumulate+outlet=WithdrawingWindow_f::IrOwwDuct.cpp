#include "uit/config/ImplSpec.hpp"
#include "uit/duct/proc/accumulating+type=fundamental/inlet=Raccumulate+outlet=WithdrawingWindow_f::IrOwwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::f::IrOwwDuct
>;

#include "../ProcDuct.hpp"
