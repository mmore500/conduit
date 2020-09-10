#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=skipping/inlet=Rsend+outlet=BlockIrecv_IrObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IrObiDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
