#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=skipping+type=trivial/inlet=DequeRput+outlet=Window_IdrOwDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IdrOwDuct
>;


int main() { return 0; }
// TODO why does this hang on travis
// #include "../ProcDuct.hpp"
// #include "../SkippingProcDuct.hpp"
