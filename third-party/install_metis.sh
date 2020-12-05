#!/bin/sh

apt-get update -qq \
  && apt-get install -qq --no-install-recommends \
    libmetis-dev=5.1.0.dfsg-5 \
    metis=5.1.0.dfsg-5 \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/* \
  || apt-get install libmetis-dev metis \
  || pacman -S metis \
  || yum install metis \
  || echo "package installer not supported! please install METIS manually"
