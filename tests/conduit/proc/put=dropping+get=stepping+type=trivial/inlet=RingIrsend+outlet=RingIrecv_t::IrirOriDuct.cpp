#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping+type=trivial/inlet=RingIrsend+outlet=RingIrecv_t::IrirOriDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IrirOriDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
