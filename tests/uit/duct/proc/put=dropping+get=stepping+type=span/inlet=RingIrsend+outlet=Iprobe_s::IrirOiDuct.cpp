#include "uit/config/ImplSpec.hpp"
#include "uit/duct/mock/ThrowDuct.hpp"
#include "uit/duct/proc/put=dropping+get=stepping+type=span/inlet=RingIrsend+outlet=Iprobe_s::IrirOiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::s::IrirOiDuct
>;

#include "../ProcDuct.hpp"
#include "../VectorProcDuct.hpp"
