#!/bin/sh

version=3.19
build=1
mkdir ~/temp
cd ~/temp
wget https://cmake.org/files/v$version/cmake-$version.$build-Linux-x86_64.sh
mkdir /opt/cmake
yes | sh cmake-$version.$build-Linux-x86_64.sh --prefix=/opt/cmake

ln -s /opt/cmake/cmake-$version.$build-Linux-x86_64/bin/cmake /usr/local/bin/cmake
