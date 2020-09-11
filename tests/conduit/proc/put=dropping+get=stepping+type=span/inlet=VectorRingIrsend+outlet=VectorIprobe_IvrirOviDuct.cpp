#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping+type=span/inlet=VectorRingIrsend+outlet=VectorIprobe_IvrirOviDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IvrirOviDuct
>;

#include "../ProcDuct.hpp"
#include "../VectorProcDuct.hpp"
