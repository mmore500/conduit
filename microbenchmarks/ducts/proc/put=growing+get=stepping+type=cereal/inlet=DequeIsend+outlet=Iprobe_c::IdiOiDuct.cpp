#include "uit/setup/ImplSpec.hpp"
#include "uit/ducts/proc/put=growing+get=stepping+type=cereal/inlet=DequeIsend+outlet=Iprobe_IdiOiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IdiOiDuct
>;

#include "../ProcDuct.hpp"
