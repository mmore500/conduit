#include "uit/setup/ImplSpec.hpp"
#include "uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=DequeRput+outlet=Window_t::IdrOwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IdrOwDuct
>;

#include "../ProcDuct.hpp"
