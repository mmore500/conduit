#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=growing+get=stepping+type=cereal/inlet=Rsend+outlet=Iprobe_c::IrOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::c::IrOiDuct
>;

#define IMPL_NAME "inlet=Rsend+outlet=Iprobe_c::IrOiDuct"

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
