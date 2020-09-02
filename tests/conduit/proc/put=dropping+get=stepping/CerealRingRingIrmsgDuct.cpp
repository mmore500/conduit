#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping/CerealRingRingIrmsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::CerealRingRingIrmsgDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
