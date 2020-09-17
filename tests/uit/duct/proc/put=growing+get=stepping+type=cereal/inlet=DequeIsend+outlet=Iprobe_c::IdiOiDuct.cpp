#include "uit/config/ImplSpec.hpp"
#include "uit/duct/mock/ThrowDuct.hpp"
#include "uit/duct/proc/put=growing+get=stepping+type=cereal/inlet=DequeIsend+outlet=Iprobe_c::IdiOiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::c::IdiOiDuct
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
