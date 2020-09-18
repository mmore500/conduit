#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=growing+get=stepping+type=trivial/inlet=Rsend+outlet=RingIrecv_t::IrOri.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IrOri
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
