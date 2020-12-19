#!/bin/sh

apt-get update -qq \
  && apt-get install -qq --no-install-recommends \
    libbenchmark1 \
    libbenchmark-dev \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/* \
  || echo "package installer not supported!"
    && echo "please install Google Benchmark manually"
