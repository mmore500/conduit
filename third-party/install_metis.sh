#!/bin/sh

apt-get install -y libmetis-dev=5.1.0.dfsg-5 metis=5.1.0.dfsg-5 \
  || apt-get install libmetis-dev metis \
  || pacman -S metis \
  || yum install metis \
  || echo "package installer not supported! please install METIS manually"
