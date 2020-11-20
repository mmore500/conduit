#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=Put+outlet=Window_t::IpOwDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IpOwDuct
>;

#define IMPL_NAME "inlet=Put+outlet=Window_t::IpOwDuct"

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
