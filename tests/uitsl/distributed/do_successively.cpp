#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/MultiprocessReporter.hpp"
#include "uitsl/distributed/do_successively.hpp"
#include "uitsl/mpi/MpiGuard.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("test do_successively") {

  uitsl::do_successively(
    [=](){ std::cout << "hello" << std::endl; },
    uitsl::print_separator
  );

}
