#!/bin/bash

git submodule update --init --recursive

./install_benchmark.sh
./install_emsdk.sh
./install_force-cover.sh
./install_mimalloc.sh
