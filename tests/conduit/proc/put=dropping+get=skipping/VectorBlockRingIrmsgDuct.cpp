#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping/VectorBlockRingIrmsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::ThrowDuct,
  uit::ThrowDuct,
  uit::VectorBlockRingIrmsgDuct
>;

#include "../FixedVectorProcDuct.hpp"
