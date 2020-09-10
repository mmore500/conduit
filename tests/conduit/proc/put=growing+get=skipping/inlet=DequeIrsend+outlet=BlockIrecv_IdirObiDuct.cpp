#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=skipping/inlet=DequeIrsend+outlet=BlockIrecv_IdirObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IdirObiDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
