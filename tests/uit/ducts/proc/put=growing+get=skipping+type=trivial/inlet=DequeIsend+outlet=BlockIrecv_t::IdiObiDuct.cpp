#include "uit/setup/ImplSpec.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=DequeIsend+outlet=BlockIrecv_t::IdiObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IdiObiDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
