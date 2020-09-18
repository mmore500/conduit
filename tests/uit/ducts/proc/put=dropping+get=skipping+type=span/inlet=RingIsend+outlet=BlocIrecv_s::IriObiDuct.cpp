#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=skipping+type=span/inlet=RingIsend+outlet=BlocIrecv_s::IriObiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::ThrowDuct,
  uit::ThrowDuct,
  uit::IriObiDuct
>;

#include "../FixedVectorProcDuct.hpp"
