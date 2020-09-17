#include "uit/config/ImplSpec.hpp"
#include "uit/duct/proc/put=dropping+get=skipping+type=trivial/inlet=RingRput+outlet=Window_t::IrrOwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IrrOwDuct
>;

#include "../ProcDuct.hpp"
