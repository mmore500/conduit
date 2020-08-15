#!/bin/sh

mkdir -p mimalloc/out/release
cd mimalloc/out/release
cmake ../..
make install

