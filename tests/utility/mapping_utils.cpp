#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/utility/mapping_utils.hpp"

TEST_CASE("Test linear encoder/decoder") {
    const size_t a = 9;
    const size_t b = 7;
    const size_t c = 2;
    const Dims dims{a, b, c};

    for (size_t i = 0; i < a * b * c ; ++i) {
        REQUIRE(uit::linear_encode(uit::linear_decode(i, dims), dims) == i);
    }
}