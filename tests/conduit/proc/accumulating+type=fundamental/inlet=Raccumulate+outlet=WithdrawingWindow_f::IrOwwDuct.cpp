#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/accumulating/inlet=Raccumulate+outlet=WithdrawingWindow_f::IrOwwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AccumulatingDuct,
  uit::ThrowDuct,
  uit::f::IrOwwDuct
>;

#include "../AccumulatingProcDuct.hpp"
