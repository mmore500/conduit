#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping/inlet=RingIsend+outlet=BlockIrecv_IriObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IriObiDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
