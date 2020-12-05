#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=skipping+type=trivial/inlet=RingIsend+outlet=BlockIrecv_t::IriObiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IriObiDuct
>;

#define IMPL_NAME "inlet=RingIsend+outlet=BlockIrecv_t::IriObiDuct"

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
