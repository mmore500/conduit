#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/proc/put=growing+get=stepping+type=cereal/inlet=DequeIsend+outlet=Iprobe_IdiOiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IdiOiDuct
>;

#include "../ProcDuct.hpp"
