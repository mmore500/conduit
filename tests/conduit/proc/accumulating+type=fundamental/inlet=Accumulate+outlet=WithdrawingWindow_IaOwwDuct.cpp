#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/intra/accumulating+type=fundamental/AccumulatingDuct.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/accumulating/inlet=Accumulate+outlet=WithdrawingWindow_IaOwwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::AccumulatingDuct,
  uit::ThrowDuct,
  uit::IaOwwDuct
>;

#include "../AccumulatingProcDuct.hpp"
