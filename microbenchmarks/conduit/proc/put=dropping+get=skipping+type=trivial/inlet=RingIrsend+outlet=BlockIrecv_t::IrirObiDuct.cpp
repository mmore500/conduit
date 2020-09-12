#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping+type=trivial/inlet=RingIrsend+outlet=BlockIrecv_t::IrirObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IrirObiDuct
>;

#include "../ProcDuct.hpp"
