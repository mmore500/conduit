#include "uit/conduit/ImplSpec.hpp"
#include "uit/conduit/intra/put=growing+get=stepping+type=any/a::DequeDuct.hpp"
#include "uit/conduit/mock/ThrowDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::DequeDuct,
  uit::ThrowDuct,
  uit::ThrowDuct
>;

#include "../IntraDuct.hpp"
#include "../SteppingIntraDuct.hpp"
#include "../ValueIntraDuct.hpp"
