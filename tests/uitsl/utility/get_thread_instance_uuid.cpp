#include <future>
#include <thread>

#include "Catch/single_include/catch2/catch.hpp"
#include "stduuid/include/uuid.h"

#include "uitsl/utility/get_thread_instance_uuid.hpp"

TEST_CASE("get_thread_instance_uuid intrathread consistency", "[nproc:1]") {
  REQUIRE(
    uitsl::get_thread_instance_uuid()
    == uitsl::get_thread_instance_uuid()
  );
}

TEST_CASE("get_thread_instance_uuid interthread differentiation", "[nproc:1]") {

  std::future<uuids::uuid> other_thread_uuid = std::async(
    [](){ return uitsl::get_thread_instance_uuid(); }
  );

  REQUIRE(
    uitsl::get_thread_instance_uuid()
    != other_thread_uuid.get()
  );

}

TEST_CASE("get_thread_instance_uuid interprocess differentiation", "[nproc:2]") {

  // TODO

}
