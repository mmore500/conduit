#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/accumulating+type=span/inlet=Isend+outlet=Irecv_s::IiOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::ThrowDuct,
  uit::ThrowDuct,
  uit::s::IiOiDuct
>;

#define MSG_VALUE_T int
#define IMPL_NAME "inlet=Isend+outlet=Irecv_s::IiOiDuct/int"

#include "../../VectorAccumulatingProcDuct.hpp"
