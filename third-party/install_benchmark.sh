#!/bin/sh

git clone https://github.com/google/googletest.git benchmark/googletest
cd benchmark
mkdir build && cd build
cmake ../
make
make test
make install
