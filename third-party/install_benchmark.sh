#!/bin/sh

git clone --recursive https://github.com/google/benchmark
cd benchmark
git reset --hard 37177a84b7e8d33696ea1e1854513cb0de3b4dc3
git clone https://github.com/google/googletest

apt-get update -qq \
  && apt-get install -qq --no-install-recommends \
    libbenchmark1 \
    libbenchmark-dev \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/* \
  || echo "package installer not supported!"
    && echo "please install Google Benchmark manually"
