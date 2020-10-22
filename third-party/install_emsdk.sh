#!/bin/bash

git clone --recursive https://github.com/emscripten-core/emsdk
cd emsdk
git reset --hard e88a3c5bbfef172a5b947768204ef734e2fb6e04
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
