#include "uit/config/ImplSpec.hpp"
#include "uit/duct/proc/put=growing+get=skipping+type=trivial/inlet=Rsend+outlet=BlockIrecv_t::IrObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IrObiDuct
>;

#include "../ProcDuct.hpp"
