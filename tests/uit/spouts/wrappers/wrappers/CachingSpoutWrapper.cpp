#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"

#include "uit/spouts/wrappers/CachingSpoutWrapper.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test CachingSpoutWrapper") {

  uit::CachingSpoutWrapper<char>{};

}
