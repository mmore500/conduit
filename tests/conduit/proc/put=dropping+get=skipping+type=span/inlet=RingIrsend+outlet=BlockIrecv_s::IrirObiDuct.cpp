#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping+type=span/inlet=RingIrsend+outlet=BlockIrecv_s::IrirObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::ThrowDuct,
  uit::ThrowDuct,
  uit::IrirObi
>;

#include "../FixedVectorProcDuct.hpp"
