#define VATD_IMPL_NAME IMPL_NAME " ValueThreadDuct"

TEST_CASE("Eventual flush-out " VATD_IMPL_NAME) { REPEAT {

  netuit::Mesh<Spec> mesh{
    netuit::DyadicTopologyFactory{}(num_threads),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    for (MSG_T i = 0; uitsl::safe_leq(i, 2 * uit::DEFAULT_BUFFER); ++i) output.TryPut(0);

    while ( !output.TryPut( 1 ) ) {
      const auto res{ input.JumpGet() }; // operational!
      REQUIRE( std::unordered_set{0, 1}.count(res) );
    }

    while ( input.JumpGet() != 1 ) {
      const auto res{ input.JumpGet() }; // operational!
      REQUIRE( std::unordered_set{0, 1}.count(res) );
    }

    REQUIRE( input.JumpGet() == 1 );

  } THREADED_END

} }


TEST_CASE("Validity " VATD_IMPL_NAME) { REPEAT {

  netuit::Mesh<Spec> mesh{
    netuit::DyadicTopologyFactory{}(num_threads),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  THREADED_BEGIN {

    auto input = mesh.GetSubmesh(thread_id)[0].GetInput(0);
    auto output = mesh.GetSubmesh(thread_id)[0].GetOutput(0);

    int last{};
    for (MSG_T msg = 0; msg < 10 * std::kilo::num; ++msg) {

      output.TryPut(msg);

      const MSG_T current = input.JumpGet();
      REQUIRE( current >= 0 );
      REQUIRE( current < 10 * std::kilo::num );
      REQUIRE( last <= current);

      last = current;

    }

  } THREADED_END

} }
