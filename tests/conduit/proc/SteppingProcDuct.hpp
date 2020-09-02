TEST_CASE("Ring Mesh connectivity") { REPEAT {

  auto [input, output] = make_ring_bundle();

  // check that everyone's connected properly
  output.TryPut(uit::get_rank());

  REQUIRE( input.GetNext() == uit::numeric_cast<MSG_T>(
    uit::circular_index(uit::get_rank(), uit::get_nprocs(), -1)
  ) );

  UIT_Barrier(MPI_COMM_WORLD); // todo why

} }

TEST_CASE("Ring Mesh sequential consistency") { {

  auto [input, output] = make_ring_bundle();

  // long enough to check that buffer wraparound works properly
  for (MSG_T i = 1; i <= 2 * DEFAULT_BUFFER; ++i) {

    UIT_Barrier( MPI_COMM_WORLD );
    output.Put(i);
    REQUIRE(input.GetNext() == i);

  }

  UIT_Barrier(MPI_COMM_WORLD); // todo why

} }

TEST_CASE("Producer-Consumer Mesh connectivity") { REPEAT {

  auto [input, output] = make_producer_consumer_bundle();

  // check that everyone's connected properly
  if (output) output->Put(uit::get_rank());

  // did we get expected rank ID as message?
  if (uit::get_nprocs() % 2 && uit::get_rank() + 1 == uit::get_nprocs()) {
    // is odd process loop at end
    REQUIRE( input->GetNext() == uit::get_rank());
  } else if (input) {
    // is consumer
    REQUIRE(
      input->GetNext() == uit::numeric_cast<int>(
        uit::circular_index(uit::get_rank(), uit::get_nprocs(), -1)
      )
    );
  } else REQUIRE( uit::get_rank() % 2 == 0 ); // is producer

  UIT_Barrier(MPI_COMM_WORLD); // todo why

} }

TEST_CASE("Producer-Consumer Mesh sequential consistency") { {

  auto [input, output] = make_producer_consumer_bundle();

  // long enough to check that buffer wraparound works properly
  for (MSG_T i = 1; i <= 2 * DEFAULT_BUFFER; ++i) {

    UIT_Barrier( MPI_COMM_WORLD );
    if (output) output->Put(i);
    if (input) REQUIRE( input->GetNext() == i );

  }

  UIT_Barrier(MPI_COMM_WORLD); // todo why

} }

TEST_CASE("Dyadic Mesh connectivity") { REPEAT {

  auto [input, output] = make_dyadic_bundle();
  UIT_Barrier(MPI_COMM_WORLD);

  // check that everyone's connected properly
  output.Put(uit::get_rank());

  // did we get expected rank ID as message?
  if (uit::get_nprocs() % 2 && uit::get_rank() + 1 == uit::get_nprocs()) {
    // is connected to self (is odd process loop at end)
    REQUIRE( input.GetNext() == uit::get_rank() );
  } else {
    // is connected to neighbor
    REQUIRE( input.GetNext() == uit::numeric_cast<int>(
      uit::circular_index(
        uit::get_rank(),
        uit::get_nprocs(),
        // is pointing forwards or backwards
        (uit::get_rank() % 2) ? -1 : 1
      )
    ) );
  }

  UIT_Barrier(MPI_COMM_WORLD); // todo why

} }

TEST_CASE("Dyadic Mesh sequential consistency") { {

  auto [input, output] = make_dyadic_bundle();

  // long enough to check that buffer wraparound works properly
  for (MSG_T i = 1; i <= 2 * DEFAULT_BUFFER; ++i) {

    UIT_Barrier( MPI_COMM_WORLD );
    output.Put(i);
    REQUIRE( input.GetNext() == i );

  }

  UIT_Barrier(MPI_COMM_WORLD); // todo why

} }
