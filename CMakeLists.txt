cmake_minimum_required(VERSION 3.13...3.18)

project(conduit VERSION 0.1.0
        DESCRIPTION ""
        LANGUAGES CXX
)

# required libraries
find_package(benchmark REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)


include(CTest)
enable_testing()

set(default_build_type "Debug")
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_FLAGS "-pipe -Wall -Wno-unused-function")
set(CATCH_BUILD_TESTING ON)
set(BUILD_TESTING ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# create conduit library
add_library(conduit INTERFACE)
# target_sources(conduit INTERFACE /include)
# add include dirs
target_include_directories(conduit INTERFACE include/)
target_include_directories(conduit INTERFACE third-party/)

target_compile_features(conduit INTERFACE cxx_std_17)

# add link flags
target_link_libraries(conduit INTERFACE stdc++fs)
target_link_libraries(conduit INTERFACE Threads::Threads)
target_link_libraries(conduit INTERFACE debug benchmark::benchmark)
target_link_libraries(conduit INTERFACE OpenMP::OpenMP_CXX)

# enable interprocedural optimization (-flto flag)
include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
#  set_target_properties(conduit PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
