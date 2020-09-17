#include "uit/config/ImplSpec.hpp"
#include "uit/duct/proc/put=growing+get=stepping+type=trivial/inlet=DequeIsend+outlet=RingIrecv_t::IdiOriDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IdiOriDuct
>;

#include "../ProcDuct.hpp"
