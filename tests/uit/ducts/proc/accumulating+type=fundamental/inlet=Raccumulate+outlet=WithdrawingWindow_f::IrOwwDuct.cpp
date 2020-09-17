#include "uit/config/ImplSpec.hpp"
#include "uit/ducts/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/accumulating/inlet=Raccumulate+outlet=WithdrawingWindow_f::IrOwwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AccumulatingDuct,
  uit::ThrowDuct,
  uit::f::IrOwwDuct
>;

#include "../AccumulatingProcDuct.hpp"
