#include "uit/config/ImplSpec.hpp"
#include "uit/duct/intra/put=growing+get=stepping+type=any/a::DequeDuct.hpp"
#include "uit/duct/mock/ThrowDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::DequeDuct,
  uit::ThrowDuct,
  uit::ThrowDuct
>;

#include "../IntraDuct.hpp"
#include "../SteppingIntraDuct.hpp"
#include "../ValueIntraDuct.hpp"
