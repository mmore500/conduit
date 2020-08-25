#!/bin/sh

git clone --recursive https://github.com/KarypisLab/METIS.git metis
cd metis
make config shared=1 cc=gcc prefix=~/local
make install

