#!/bin/sh

git clone --recursive https://github.com/google/benchmark
cd benchmark
git reset --hard 37177a84b7e8d33696ea1e1854513cb0de3b4dc3
git clone https://github.com/google/googletest
mkdir build && cd build
cmake ../
make
make test
make install
