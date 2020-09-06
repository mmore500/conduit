#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping/VectorBlockRingImsgDuct.hpp"
// #include "uit/conduit/proc/put=dropping+get=stepping/VectorRingImsgDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::ThrowDuct,
  uit::ThrowDuct,
  uit::VectorBlockRingImsgDuct
>;

#include "../FixedVectorProcDuct.hpp"
