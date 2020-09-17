#include "uit/config/ImplSpec.hpp"
#include "uit/duct/mock/ThrowDuct.hpp"
#include "uit/duct/proc/put=dropping+get=stepping+type=trivial/pooled+inlet=RingIsend+outlet=Iprobe_t::PooledIriOiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::PooledIriOiDuct
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
