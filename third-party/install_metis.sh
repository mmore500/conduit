#!/bin/sh

apt-get install libmetis-lib metis ||\
pacman -S metis ||\
yum install metis ||\
echo "package installer not supported! please install METIS manually"
