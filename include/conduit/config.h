#pragma once

namespace uit {

#define DEFAULT_BUFFER 1024

#ifdef __cpp_lib_hardware_interference_size
  static constexpr size_t CACHE_LINE_SIZE =
    std::hardware_destructive_interference_size;
#else
  static constexpr size_t CACHE_LINE_SIZE = 64;
#endif

#ifndef ProcInletDuct
  #define ProcInletDuct ISendDuct
#endif

#ifndef ProcOutletDuct
  #define ProcOutletDuct IRecvDuct
#endif

#ifndef ThreadDuct
  #define ThreadDuct HeadTailDuct
#endif

#ifndef IntraDuct
  #define IntraDuct PendingDuct
#endif

}
