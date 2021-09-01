#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/ducts/Duct.hpp"
#include "uit/setup/ImplSelect.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/spouts/Outlet.hpp"
#include "uit/spouts/wrappers/InstrumentationAggregatingSpoutWrapper.hpp"
#include "uit/spouts/wrappers/outlet/InstrumentationAggregatingOutletWrapper.hpp"

TEST_CASE("Test InstrumentationAggregatingOutletWrapper") {

  using Spec = uit::ImplSpec<
    char,
    uit::ImplSelect<>,
    uit::InstrumentationAggregatingSpoutWrapper
  >;
  uit::internal::InstrumentationAggregatingOutletWrapper< uit::Outlet< Spec > >{
    std::make_shared<uit::internal::Duct<Spec>>()
  };

}
