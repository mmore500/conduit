#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping+type=trivial/PooledRingImsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::PooledIvriOviDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
