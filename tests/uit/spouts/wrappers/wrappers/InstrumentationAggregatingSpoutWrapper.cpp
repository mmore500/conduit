#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/spouts/wrappers/InstrumentationAggregatingSpoutWrapper.hpp"

TEST_CASE("Test InstrumentationAggregatingSpoutWrapper") {

  uit::InstrumentationAggregatingSpoutWrapper<char>{};

}
