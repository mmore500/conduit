#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping/PooledBlockRingImsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::PooledBlockRingImsgDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
