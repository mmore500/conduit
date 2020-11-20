#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=growing+get=stepping+type=trivial/inlet=DequeIsend+outlet=RingIrecv_t::IdiOriDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IdiOriDuct
>;

#define IMPL_NAME "inlet=DequeIsend+outlet=RingIrecv_t::IdiOriDuct"

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
