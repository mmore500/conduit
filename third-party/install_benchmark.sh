#!/bin/sh

# adapted from https://stackoverflow.com/a/43136160
mkdir benchmark && cd benchmark
git init
git remote add origin https://github.com/google/benchmark
git fetch --depth 1 origin 37177a84b7e8d33696ea1e1854513cb0de3b4dc3
git checkout FETCH_HEAD

# adapted from https://stackoverflow.com/a/43136160
mkdir googletest && cd googletest
git init
git remote add origin https://github.com/google/googletest
git fetch --depth 1 origin ca4b7c9ff4d8a5c37ac51795b03ffe934958aeff
git checkout FETCH_HEAD
cd ..

cmake -E make_directory "build"
cmake -E chdir "build" cmake -DCMAKE_BUILD_TYPE=Release ../
cmake --build "build" --config Release --target install

# pacakge manger install isn't new enough, disable it
# apt-get update -qq \
#   && apt-get install -qq --no-install-recommends \
#     libbenchmark1 \
#     libbenchmark-dev \
#   && apt-get clean \
#   && rm -rf /var/lib/apt/lists/* \
#   || echo "package installer not supported!"
#     && echo "please install Google Benchmark manually"
