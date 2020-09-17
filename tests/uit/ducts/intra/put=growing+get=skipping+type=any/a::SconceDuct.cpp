#include "uit/config/ImplSpec.hpp"
#include "uit/ducts/intra/put=growing+get=skipping+type=any/a::SconceDuct.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SconceDuct,
  uit::ThrowDuct,
  uit::ThrowDuct
>;

#include "../IntraDuct.hpp"
#include "../ValueIntraDuct.hpp"
