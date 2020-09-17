#include "uit/config/ImplSpec.hpp"
#include "uit/duct/proc/put=dropping+get=stepping+type=trivial/inlet=RingIrsend+outlet=RingIrecv_t::IrirOriDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IrirOriDuct
>;

#include "../ProcDuct.hpp"
