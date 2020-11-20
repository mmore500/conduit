#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=skipping+type=trivial/inlet=RingRput+outlet=Window_t::IrrOwDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IrrOwDuct
>;

#define IMPL_NAME "inlet=RingRput+outlet=Window_t::IrrOwDuct"

// TODO why does this hang on travis?
// #include "../ProcDuct.hpp"
// #include "../SkippingProcDuct.hpp"
