#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=trivial/buffered+inlet=RingIsend+outlet=Iprobe_t::BufferedIriOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::BufferedIriOiDuct
>;

#define IMPL_NAME "buffered+inlet=RingIsend+outlet=Iprobe_t::BufferedIriOiDuct"

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
