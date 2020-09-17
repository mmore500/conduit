#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping+type=span/inlet=RingIsend+outlet=BlocIrecv_s::IriObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::ThrowDuct,
  uit::ThrowDuct,
  uit::IriObiDuct
>;

#include "../FixedVectorProcDuct.hpp"
