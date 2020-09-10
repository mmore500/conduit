#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping/inlet=VectorRingIsend+outlet=VectorIprobe_IvriOviDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::ThrowDuct,
  uit::IvriOviDuct
>;

#include "../VectorProcDuct.hpp"
