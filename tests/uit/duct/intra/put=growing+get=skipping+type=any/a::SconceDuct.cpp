#include "uit/config/ImplSpec.hpp"
#include "uit/duct/intra/put=growing+get=skipping+type=any/a::SconceDuct.hpp"
#include "uit/duct/mock/ThrowDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SconceDuct,
  uit::ThrowDuct,
  uit::ThrowDuct
>;

#include "../IntraDuct.hpp"
#include "../ValueIntraDuct.hpp"
