#!/bin/bash

git submodule deinit -f .
git submodule init
git submodule update -f

./install_benchmark.sh
./install_emsdk.sh
./install_force-cover.sh
