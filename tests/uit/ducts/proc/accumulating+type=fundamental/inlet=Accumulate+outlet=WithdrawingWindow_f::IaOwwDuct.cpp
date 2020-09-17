#include "uit/config/ImplSpec.hpp"
#include "uit/ducts/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/accumulating/inlet=Accumulate+outlet=WithdrawingWindow_f::IaOwwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::AccumulatingDuct,
  uit::ThrowDuct,
  uit::f::IaOwwDuct
>;

#include "../AccumulatingProcDuct.hpp"
