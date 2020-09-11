#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping+type=trivial/PooledRingImsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::PooledIvriOviDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
