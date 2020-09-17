#pragma once

#include "OccupancyCaps.hpp"
#include "OccupancyGuard.hpp"

#ifdef UIT_AUDIT_OCCUPANCY

  #define uit_occupancy_auditor \
    uitsl::OccupancyCaps __uit_occupancy_auditor__

  #define uit_occupancy_audit(count) \
    const uitsl::OccupancyGuard guard{ \
      __uit_occupancy_auditor__.Get(__LINE__, count) \
    }

#else

  #define uit_occupancy_auditor \
    static_assert( \
      true, \
      "semi-colon required after this macro, adapted from https://stackoverflow.com/a/59153563/1261599" \
    )

  #define uit_occupancy_audit(count) \
    static_assert( \
      true, \
      "semi-colon required after this macro, adapted from https://stackoverflow.com/a/59153563/1261599" \
    )

#endif
