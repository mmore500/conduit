#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=growing+get=stepping+type=trivial/inlet=DequeIrsend+outlet=RingIrecv_t::IdirOriDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IsOriDuct
>;

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
