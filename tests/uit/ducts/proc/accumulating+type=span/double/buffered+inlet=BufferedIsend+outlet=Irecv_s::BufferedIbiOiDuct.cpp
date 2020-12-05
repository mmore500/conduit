#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/accumulating+type=span/buffered+inlet=BufferedIsend+outlet=Irecv_s::BufferedIbiOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::ThrowDuct,
  uit::ThrowDuct,
  uit::s::BufferedIbiOiDuct
>;

#define MSG_VALUE_T double
#define IMPL_NAME "buffered+inlet=BufferedIsend+outlet=Irecv_s::BufferedIbiOiDuct/double"

#include "../../VectorAccumulatingProcDuct.hpp"
