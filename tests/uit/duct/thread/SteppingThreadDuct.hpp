TEST_CASE("Ring Mesh connectivity") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_threads),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    // check that everyone's connected properly
    output.TryPut(uitsl::get_rank());

    REQUIRE( input.GetNext() == uitsl::safe_cast<MSG_T>(
      uitsl::circular_index(uitsl::get_rank(), uitsl::get_nprocs(), -1)
    ) );

  } THREADED_END

} }

TEST_CASE("Ring Mesh sequential consistency") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_threads),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    // long enough to check that buffer wraparound works properly
    for (MSG_T i = 1; i <= 2 * uit::DEFAULT_BUFFER; ++i) {

      barrier->arrive_and_wait();

      output.Put(i);
      REQUIRE(input.GetNext() == i);

    }

  } THREADED_END

} }

TEST_CASE("Producer-Consumer Mesh connectivity") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_threads),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInputOrNullopt(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutputOrNullopt(0);

    // check that everyone's connected properly
    if (output) output->Put(uitsl::get_rank());

    // did we get expected rank ID as message?
    if (uitsl::get_nprocs() % 2 && uitsl::get_rank() + 1 == uitsl::get_nprocs()) {
      // is odd process loop at end
      REQUIRE( input->GetNext() == uitsl::get_rank());
    } else if (input) {
      // is consumer
      REQUIRE(
        input->GetNext() == uitsl::safe_cast<int>(
          uitsl::circular_index(uitsl::get_rank(), uitsl::get_nprocs(), -1)
        )
      );
    } else REQUIRE( uitsl::get_rank() % 2 == 0 ); // is producer

  } THREADED_END

} }

TEST_CASE("Producer-Consumer Mesh sequential consistency") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_threads),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInputOrNullopt(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutputOrNullopt(0);

    // long enough to check that buffer wraparound works properly
    for (MSG_T i = 1; i <= 2 * uit::DEFAULT_BUFFER; ++i) {

      if (output) output->Put(i);
      if (input) REQUIRE( input->GetNext() == i );

    }

  } THREADED_END

} }

TEST_CASE("Dyadic Mesh connectivity") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::DyadicTopologyFactory{}(num_threads),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    // check that everyone's connected properly
    output.Put(uitsl::get_rank());

    // did we get expected rank ID as message?
    if (uitsl::get_nprocs() % 2 && uitsl::get_rank() + 1 == uitsl::get_nprocs()) {
      // is connected to self (is odd process loop at end)
      REQUIRE( input.GetNext() == uitsl::get_rank() );
    } else {
      // is connected to neighbor
      REQUIRE( input.GetNext() == uitsl::safe_cast<int>(
        uitsl::circular_index(
          uitsl::get_rank(),
          uitsl::get_nprocs(),
          // is pointing forwards or backwards
          (uitsl::get_rank() % 2) ? -1 : 1
        )
      ) );
    }

  } THREADED_END

} }

TEST_CASE("Dyadic Mesh sequential consistency") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::DyadicTopologyFactory{}(num_threads),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    // long enough to check that buffer wraparound works properly
    for (MSG_T i = 1; i <= 2 * uit::DEFAULT_BUFFER; ++i) {

      barrier->arrive_and_wait();

      output.Put(i);
      REQUIRE( input.GetNext() == i );

    }

  } THREADED_END

} }
