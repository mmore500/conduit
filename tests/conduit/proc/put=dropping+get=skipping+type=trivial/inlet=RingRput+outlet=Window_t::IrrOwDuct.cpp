#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping+type=trivial/inlet=RingRput+outlet=Window_t::IrrOwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IrrOwDuct
>;

int main() { return 0; }

// TODO why does this hang on travis?
// #include "../ProcDuct.hpp"
// #include "../SkippingProcDuct.hpp"
