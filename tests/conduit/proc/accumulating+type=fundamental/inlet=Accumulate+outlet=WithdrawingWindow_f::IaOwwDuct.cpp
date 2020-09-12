#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/accumulating/inlet=Accumulate+outlet=WithdrawingWindow_f::IaOwwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::AccumulatingDuct,
  uit::ThrowDuct,
  uit::f::IaOwwDuct
>;

#include "../AccumulatingProcDuct.hpp"
