#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/intra/put=growing+get=skipping/SconceDuct.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::SconceDuct,
  uit::ThrowDuct,
  uit::ThrowDuct
>;

#include "../IntraDuct.hpp"
