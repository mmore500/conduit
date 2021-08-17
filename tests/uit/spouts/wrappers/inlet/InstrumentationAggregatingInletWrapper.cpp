#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/ducts/Duct.hpp"
#include "uit/setup/ImplSelect.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/spouts/Inlet.hpp"
#include "uit/spouts/wrappers/inlet/InstrumentationAggregatingInletWrapper.hpp"
#include "uit/spouts/wrappers/InstrumentationAggregatingSpoutWrapper.hpp"

TEST_CASE("Test InstrumentationAggregatingInletWrapper") {

  using Spec = uit::ImplSpec<
    char,
    uit::ImplSelect<>,
    uit::InstrumentationAggregatingSpoutWrapper
  >;
  uit::internal::InstrumentationAggregatingInletWrapper< uit::Inlet< Spec > >{
    std::make_shared<uit::internal::Duct<Spec>>()
  };

}
