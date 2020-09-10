#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping/inlet=VectorRingIsend+outlet=VectorBlocIrecv_IvriOvbiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::ThrowDuct,
  uit::ThrowDuct,
  uit::IvriOvbiDuct
>;

#include "../FixedVectorProcDuct.hpp"
