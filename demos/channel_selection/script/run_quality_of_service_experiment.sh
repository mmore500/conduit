#!/bin/bash

# resource allocation and setup
# (do this manually and be sure to fill in SET_ME)
: << '###BLOCK-COMMENT'
salloc --nodes=SET_ME --ntasks=SET_ME --cpus-per-task=1 --time=4:00:00 --mem-per-cpu=4G --constraint=lac
source ~/.secrets.sh
singularity exec docker://mmore500/dishtiny:sha-2f60ec6 make
module purge
module load GCC/7.2.0-2.29
module load MPICH/3.2.1
###BLOCK-COMMENT

for REPLICATE in 0 1 2 3 4 5 6 7 8 9; do
  for ASYNCHRONOUS in 3; do
    for N_PROCS in 256 128 64 32; do
      echo "REPLICATE ${REPLICATE}"
      echo "ASYNCHRONOUS ${ASYNCHRONOUS}"
      echo "N_PROCS ${N_PROCS}"

      export UITSL_EXEC_INSTANCE_UUID="$(cat /proc/sys/kernel/random/uuid)"
      mpiexec -n "${N_PROCS}" \
        singularity exec docker://mmore500/dishtiny:sha-2f60ec6 \
        ./channel_selection \
          -RUN_SECONDS 330 \
          -REPLICATE "${REPLICATE}" \
          -ASYNCHRONOUS "${ASYNCHRONOUS}"
    done
  done
done
