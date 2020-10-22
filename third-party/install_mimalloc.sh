#!/bin/sh

git clone --recursive https://github.com/microsoft/mimalloc
cd mimalloc
git reset --hard a9686d6ecf00e4467e772f7c0b4ef76a15f325f6
mkdir -p out/release
cd out/release
cmake ../..
make install
