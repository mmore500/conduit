#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=skipping+type=trivial/buffered+inlet=RingIsend+outlet=BlockIrecv_t::BufferedIriObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::BufferedIriObiDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
