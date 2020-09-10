#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=stepping/inlet=Rsend+outlet=RingIrecv+IrOri.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IrOri
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
