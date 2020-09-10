#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/intra/accumulating/AccumulatingDuct.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/accumulating/inlet=Raccumulate+outlet=WithdrawingWindow_IrOwwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::AccumulatingDuct,
  uit::ThrowDuct,
  uit::IrOwwDuct
>;

#include "AccumulatingDuct.hpp"
