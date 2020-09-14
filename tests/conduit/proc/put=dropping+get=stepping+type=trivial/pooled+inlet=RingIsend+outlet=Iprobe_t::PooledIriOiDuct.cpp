#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping+type=trivial/pooled+inlet=RingIsend+outlet=Iprobe_t::PooledIriOiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::PooledIriOiDuct
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
