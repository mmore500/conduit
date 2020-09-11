#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=skipping+type=trivial/inlet=Send+outlet=BlockIrecv_IsObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IsObiDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
