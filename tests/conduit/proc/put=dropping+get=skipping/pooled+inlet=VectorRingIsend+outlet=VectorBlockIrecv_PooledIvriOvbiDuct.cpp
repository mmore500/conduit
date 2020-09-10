#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping/pooled+inlet=VectorRingIsend+outlet=VectorBlockIrecv_PooledIvriOvbiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::PooledIvriOvbiDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
