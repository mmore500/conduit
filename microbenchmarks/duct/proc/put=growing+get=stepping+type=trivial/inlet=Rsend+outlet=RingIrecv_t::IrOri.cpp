#include "uit/config/ImplSpec.hpp"
#include "uit/duct/proc/put=growing+get=stepping+type=trivial/inlet=Rsend+outlet=RingIrecv_t::IrOri.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IrOri
>;

#include "../ProcDuct.hpp"
