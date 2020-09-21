#pragma once

#include "uit/ducts/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/ducts/proc/accumulating+type=fundamental/aggregated+inlet=Raccumulate+outlet=WithdrawingWindow_f::AggregatedIrOwwDuct.hpp"
#include "uit/ducts/proc/accumulating+type=fundamental/inlet=Raccumulate+outlet=WithdrawingWindow_f::IrOwwDuct.hpp"
#include "uit/ducts/thread/accumulating+type=any/a::MutexAccumulatingDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AccumulatingDuct,
  uit::a::MutexAccumulatingDuct,
  uit::f::AggregatedIrOwwDuct
>;

using ImplSpec = uit::ImplSpec<
  message_t,
  ImplSel,
  2, // N
  16 // B
>;
