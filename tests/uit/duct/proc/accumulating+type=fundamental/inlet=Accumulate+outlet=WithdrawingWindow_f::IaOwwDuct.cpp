#include "uit/config/ImplSpec.hpp"
#include "uit/duct/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/duct/mock/ThrowDuct.hpp"
#include "uit/duct/proc/accumulating/inlet=Accumulate+outlet=WithdrawingWindow_f::IaOwwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::AccumulatingDuct,
  uit::ThrowDuct,
  uit::f::IaOwwDuct
>;

#include "../AccumulatingProcDuct.hpp"
