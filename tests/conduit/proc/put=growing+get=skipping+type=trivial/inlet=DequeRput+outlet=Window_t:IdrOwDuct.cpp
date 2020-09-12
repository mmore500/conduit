#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=skipping+type=trivial/inlet=DequeRput+outlet=Window_t::IdrOwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IdrOwDuct
>;


int main() { return 0; }
// TODO why does this hang on travis
// #include "../ProcDuct.hpp"
// #include "../SkippingProcDuct.hpp"
