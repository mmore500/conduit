#include "uit/ducts/intra/put=growing+get=skipping+type=any/a::SconceDuct.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SconceDuct,
  uit::ThrowDuct,
  uit::ThrowDuct
>;

#define MSG_T int

#include "../IntraDuct.hpp"
#include "../ValueIntraDuct.hpp"
