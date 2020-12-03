#!/bin/sh

# Clone Google Benchmark
git clone --recursive https://github.com/google/benchmark
# Go to the library root directory
cd benchmark
# Use pinned commit
git reset --hard 37177a84b7e8d33696ea1e1854513cb0de3b4dc3
# Clone Google Test (as a dependency)
git clone https://github.com/google/googletest
# Make a build directory to place the build output.
cmake -E make_directory "build"
# Generate build system files with cmake.
cmake -DCMAKE_BUILD_TYPE=Release -S . -B "build"
# Build the library.
cmake --build "build" --config Release
