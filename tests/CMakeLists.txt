find_package(Catch2 REQUIRED)

file(GLOB_RECURSE all_TESTS
    *.cpp
)

set(CATCH_BUILD_TESTING ON)
set(BUILD_TESTING ON)

add_executable(tests
    #uitsl/math/math_utils.cpp
    ${all_TESTS}
)
target_link_libraries(tests Catch2::Catch2 conduit)

include(ParseAndAddCatchTests)
include(Catch)
ParseAndAddCatchTests(tests)
#catch_discover_tests(tests)

