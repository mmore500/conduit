#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/intra/accumulating/AccumulatingDuct.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/accumulating/RaccumulateWithdrawingDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::AccumulatingDuct,
  uit::ThrowDuct,
  uit::RaccumulateWithdrawingDuct
>;

#include "AccumulatingDuct.hpp"
