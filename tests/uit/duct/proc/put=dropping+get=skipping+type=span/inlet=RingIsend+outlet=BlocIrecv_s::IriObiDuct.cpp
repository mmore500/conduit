#include "uit/config/ImplSpec.hpp"
#include "uit/duct/mock/ThrowDuct.hpp"
#include "uit/duct/proc/put=dropping+get=skipping+type=span/inlet=RingIsend+outlet=BlocIrecv_s::IriObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::ThrowDuct,
  uit::ThrowDuct,
  uit::IriObiDuct
>;

#include "../FixedVectorProcDuct.hpp"
