#pragma once
#ifndef UIT_EMP_VENDORIZATION_CONSOLIDATE_CONTEXT_SWITCHES_HPP_INCLUDE
#define UIT_EMP_VENDORIZATION_CONSOLIDATE_CONTEXT_SWITCHES_HPP_INCLUDE

// this header can be included by end users at the top of the translation unit
// to prevent macro inseep into the vendorized implementation

#ifdef UIT_VENDORIZE_EMP
#include "push_uit_emp.hh"
#endif // #ifdef UIT_VENDORIZE_EMP

#include "../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../third-party/Empirical/include/emp/config/config.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataNode.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/Empirical/include/emp/datastructs/Cache.hpp"
#include "../../../third-party/Empirical/include/emp/datastructs/hash_utils.hpp"
#include "../../../third-party/Empirical/include/emp/datastructs/tuple_utils.hpp"
#include "../../../third-party/Empirical/include/emp/datastructs/QueueCache.hpp"
#include "../../../third-party/Empirical/include/emp/io/ContiguousStream.hpp"
#include "../../../third-party/Empirical/include/emp/io/MemoryIStream.hpp"
#include "../../../third-party/Empirical/include/emp/matching/matchbin_metrics.hpp"
#include "../../../third-party/Empirical/include/emp/matching/MatchDepository.hpp"
#include "../../../third-party/Empirical/include/emp/matching/regulators/PlusCountdownRegulator.hpp"
#include "../../../third-party/Empirical/include/emp/matching/selectors_static/RankedSelector.hpp"
#include "../../../third-party/Empirical/include/emp/math/Distribution.hpp"
#include "../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../third-party/Empirical/include/emp/math/Random.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"
#include "../../../third-party/Empirical/include/emp/meta/meta.hpp"
#include "../../../third-party/Empirical/include/emp/meta/TypePack.hpp"
#include "../../../third-party/Empirical/include/emp/polyfill/span.hpp"
#include "../../../third-party/Empirical/include/emp/tools/hash_namify.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#ifndef UIT_VENDORIZE_EMP
namespace emp {}
namespace uit_emp = emp;
#endif // #ifndef UIT_VENDORIZE_EMP

#ifdef UIT_VENDORIZE_EMP
#include "pop_uit_emp.hh"
#endif // #ifdef UIT_VENDORIZE_EMP

#endif // #ifndef UIT_EMP_VENDORIZATION_CONSOLIDATE_CONTEXT_SWITCHES_HPP_INCLUDE
