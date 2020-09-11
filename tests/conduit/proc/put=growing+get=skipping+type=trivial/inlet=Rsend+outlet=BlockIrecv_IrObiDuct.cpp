#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=skipping+type=trivial/BlockRmsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::BlockRmsgDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
