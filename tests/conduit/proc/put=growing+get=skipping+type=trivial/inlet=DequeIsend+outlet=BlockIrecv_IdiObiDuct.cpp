#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=skipping+type=trivial/inlet=DequeIsend+outlet=BlockIrecv_IdiObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IdiObiDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
