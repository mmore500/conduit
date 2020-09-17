#include "uit/config/ImplSpec.hpp"
#include "uit/duct/proc/put=dropping+get=stepping+type=cereal/inlet=RingIsend+outlet=Iprobe_c::IriOiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::c::IriOiDuct
>;

#include "../ProcDuct.hpp"
