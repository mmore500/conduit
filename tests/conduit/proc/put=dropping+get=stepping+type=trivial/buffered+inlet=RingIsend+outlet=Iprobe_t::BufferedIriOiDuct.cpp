#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"
#include "uit/conduit/proc/put=dropping+get=stepping+type=trivial/buffered+inlet=RingIsend+outlet=Iprobe_t::BufferedIriOiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::BufferedIriOiDuct
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
