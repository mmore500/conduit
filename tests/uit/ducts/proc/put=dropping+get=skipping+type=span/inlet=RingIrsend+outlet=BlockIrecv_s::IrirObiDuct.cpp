#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=skipping+type=span/inlet=RingIrsend+outlet=BlockIrecv_s::IrirObiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::ThrowDuct,
  uit::ThrowDuct,
  uit::IrirObi
>;

#include "../FixedVectorProcDuct.hpp"
