#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=growing+get=stepping+type=cereal/inlet=Send+outlet=Iprobe_c::IsOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::c::IsOiDuct
>;

#define IMPL_NAME "inlet=Send+outlet=Iprobe_c::IsOiDuct"

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
