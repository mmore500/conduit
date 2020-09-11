#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=skipping+type=trivial/inlet=Put+outlet=Window_IpOwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IpOwDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
