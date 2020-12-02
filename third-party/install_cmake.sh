#!/bin/sh

wget https://github.com/Kitware/CMake/releases/download/v3.19.1/cmake-3.19.1.tar.gz
tar -zxvf cmake-3.19.1.tar.gz
cd cmake-3.19.1
./bootstrap
make
make install
