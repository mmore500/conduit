#include "uit/config/ImplSpec.hpp"
#include "uit/duct/proc/put=dropping+get=stepping+type=cereal/inlet=ringIrsend+outlet=Iprobe_c::IrirOiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::c::IrirOiDuct
>;

#include "../ProcDuct.hpp"
