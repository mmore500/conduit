#!/bin/sh

apt-get install libmetis-dev metis ||\
pacman -S metis ||\
yum install metis ||\
echo "package installer not supported! please install METIS manually"
