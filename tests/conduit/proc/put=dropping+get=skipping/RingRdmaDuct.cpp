#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping/RingRdmaDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::RingRdmaDuct
>;

#include "../ProcDuct.hpp"
// TODO why does this hang on travis?
// #include "../SkippingProcDuct.hpp"
