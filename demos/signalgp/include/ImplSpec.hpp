#pragma once

#include "uit/setup/ImplSpec.hpp"

#include "uit/ducts/proc/put=dropping+get=stepping+type=cereal/aggregated+inlet=RingIsend+outlet=Iprobe_c::AggregatedIriOiDuct.hpp"

using ImplSel = uit::ImplSelect<
  uit::DefaultIntraDuct,
  uit::DefaultThreadDuct,
  uit::c::AggregatedIriOiDuct
>;

using ImplSpec = uit::ImplSpec<
  message_t,
  2, // N
  ImplSel,
  16 // B
>;
