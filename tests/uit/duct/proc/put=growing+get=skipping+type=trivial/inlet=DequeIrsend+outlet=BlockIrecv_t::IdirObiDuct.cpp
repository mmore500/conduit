#include "uit/config/ImplSpec.hpp"
#include "uit/duct/mock/ThrowDuct.hpp"
#include "uit/duct/proc/put=growing+get=skipping+type=trivial/inlet=DequeIrsend+outlet=BlockIrecv_t::IdirObiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IdirObiDuct
>;

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
