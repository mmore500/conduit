TEST_CASE("Ring Mesh connectivity") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    // check that everyone's connected properly
    output.TryPut(uit::get_rank());

    REQUIRE( input.GetNext() == uit::safe_cast<MSG_T>(
      uit::circular_index(uit::get_rank(), uit::get_nprocs(), -1)
    ) );

  } THREADED_END

} }

TEST_CASE("Ring Mesh sequential consistency") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    // long enough to check that buffer wraparound works properly
    for (MSG_T i = 1; i <= 2 * DEFAULT_BUFFER; ++i) {

      barrier->arrive_and_wait();

      output.Put(i);
      REQUIRE(input.GetNext() == i);

    }

  } THREADED_END

} }

TEST_CASE("Producer-Consumer Mesh connectivity") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInputOrNullopt(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutputOrNullopt(0);

    // check that everyone's connected properly
    if (output) output->Put(uit::get_rank());

    // did we get expected rank ID as message?
    if (uit::get_nprocs() % 2 && uit::get_rank() + 1 == uit::get_nprocs()) {
      // is odd process loop at end
      REQUIRE( input->GetNext() == uit::get_rank());
    } else if (input) {
      // is consumer
      REQUIRE(
        input->GetNext() == uit::safe_cast<int>(
          uit::circular_index(uit::get_rank(), uit::get_nprocs(), -1)
        )
      );
    } else REQUIRE( uit::get_rank() % 2 == 0 ); // is producer

  } THREADED_END

} }

TEST_CASE("Producer-Consumer Mesh sequential consistency") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::RingTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInputOrNullopt(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutputOrNullopt(0);

    // long enough to check that buffer wraparound works properly
    for (MSG_T i = 1; i <= 2 * DEFAULT_BUFFER; ++i) {

      if (output) output->Put(i);
      if (input) REQUIRE( input->GetNext() == i );

    }

  } THREADED_END

} }

TEST_CASE("Dyadic Mesh connectivity") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::DyadicTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    // check that everyone's connected properly
    output.Put(uit::get_rank());

    // did we get expected rank ID as message?
    if (uit::get_nprocs() % 2 && uit::get_rank() + 1 == uit::get_nprocs()) {
      // is connected to self (is odd process loop at end)
      REQUIRE( input.GetNext() == uit::get_rank() );
    } else {
      // is connected to neighbor
      REQUIRE( input.GetNext() == uit::safe_cast<int>(
        uit::circular_index(
          uit::get_rank(),
          uit::get_nprocs(),
          // is pointing forwards or backwards
          (uit::get_rank() % 2) ? -1 : 1
        )
      ) );
    }

  } THREADED_END

} }

TEST_CASE("Dyadic Mesh sequential consistency") { REPEAT {

  uit::Mesh<Spec> mesh{
    uit::DyadicTopologyFactory{}(num_threads),
    uit::AssignSegregated<uit::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    // long enough to check that buffer wraparound works properly
    for (MSG_T i = 1; i <= 2 * DEFAULT_BUFFER; ++i) {

      barrier->arrive_and_wait();

      output.Put(i);
      REQUIRE( input.GetNext() == i );

    }

  } THREADED_END

} }
