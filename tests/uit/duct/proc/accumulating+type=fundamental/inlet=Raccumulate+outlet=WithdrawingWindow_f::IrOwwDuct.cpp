#include "uit/config/ImplSpec.hpp"
#include "uit/duct/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/duct/mock/ThrowDuct.hpp"
#include "uit/duct/proc/accumulating/inlet=Raccumulate+outlet=WithdrawingWindow_f::IrOwwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AccumulatingDuct,
  uit::ThrowDuct,
  uit::f::IrOwwDuct
>;

#include "../AccumulatingProcDuct.hpp"
