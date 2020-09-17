#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/mpi/routine_functors.hpp"
#include "uitsl/mpi/MpiGuard.hpp"
#include "uitsl/debug/MultiprocessReporter.hpp"

const uit::MpiGuard guard;

TEST_CASE("Test IrsendFunctor") {

  // TODO flesh out stub test
  uit::IrsendFunctor{};

}

TEST_CASE("Test IsendFunctor") {

  // TODO flesh out stub test
  uit::IsendFunctor{};

}

TEST_CASE("Test RsendFunctor") {

  // TODO flesh out stub test
  uit::RsendFunctor{};

}

TEST_CASE("Test SendFunctor") {

  // TODO flesh out stub test
  uit::SendFunctor{};

}
