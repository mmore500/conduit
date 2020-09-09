#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=growing+get=skipping/DequeRdmaDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::DequeRdmaDuct
>;


int main() { return 0; }
// TODO why does this hang on travis
// #include "../ProcDuct.hpp"
// #include "../SkippingProcDuct.hpp"
