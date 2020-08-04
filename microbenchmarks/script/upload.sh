#!/bin/bash

VERSION=$(git describe --dirty --always)
TIME=$(date +%s)
NNODE=$([ -z "$SLURM_JOB_NUM_NODES" ] && echo 1 || echo $SLURM_JOB_NUM_NODES)
NPROC=$(nproc)

OSF_PATH="microbenchmarks/context=${CONTEXT}/nnode=${NNODE}+nproc=${NPROC}/time=${TIME}+version=${VERSION}"

set -o xtrace

for f in */*/*.json *.csv *.png; do
  osf -u mmore500.login@gmail.com -p 7jkgp upload $f $OSF_PATH/$f;
done
