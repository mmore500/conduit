#include "uit/config/ImplSpec.hpp"
#include "uit/duct/proc/put=growing+get=skipping+type=trivial/inlet=Put+outlet=Window_t::IpOwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IpOwDuct
>;

#include "../ProcDuct.hpp"
