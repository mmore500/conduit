#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/distributed/do_successively.hpp"


TEST_CASE("test do_successively") {

  uitsl::do_successively(
    [=](){ std::cout << "hello" << std::endl; },
    uitsl::print_separator
  );

}
