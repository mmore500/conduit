#include "uit/config/ImplSpec.hpp"
#include "uit/duct/proc/put=growing+get=skipping+type=trivial/inlet=DequeIsend+outlet=BlockIrecv_t::IdiObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IdiObiDuct
>;

#include "../ProcDuct.hpp"
