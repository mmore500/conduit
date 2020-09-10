#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping/inlet=VectorRingIrsend+outlet=VectorBlockIrecv_IvrirOvbiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::ThrowDuct,
  uit::ThrowDuct,
  uit::IvrirOvbi
>;

#include "../FixedVectorProcDuct.hpp"
