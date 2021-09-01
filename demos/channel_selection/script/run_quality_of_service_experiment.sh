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
export SLURM_NNODES_OVERRIDE=SET_ME
###BLOCK-COMMENT

# weak scaling
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

# multithreading vs multiprocessing
for REPLICATE in 0 1 2 3 4 5 6 7 8 9; do
  for ASYNCHRONOUS in 3; do
    export UITSL_EXEC_INSTANCE_UUID="$(cat /proc/sys/kernel/random/uuid)"
    mpiexec -n 2 \
      singularity exec docker://mmore500/dishtiny:sha-2f60ec6 \
      ./channel_selection \
        -RUN_SECONDS 330 \
        -REPLICATE "${REPLICATE}" \
        -N_THREADS 1 \
        -ASYNCHRONOUS "${ASYNCHRONOUS}" \
        -WHICH_TOPOLOGY "dyadic" \
        -N_NODES_PER_CPU 1

    export UITSL_EXEC_INSTANCE_UUID="$(cat /proc/sys/kernel/random/uuid)"
    mpiexec -n 1 \
      singularity exec docker://mmore500/dishtiny:sha-2f60ec6 \
      ./channel_selection \
        -RUN_SECONDS 330 \
        -REPLICATE "${REPLICATE}" \
        -N_THREADS 2 \
        -ASYNCHRONOUS "${ASYNCHRONOUS}" \
        -WHICH_TOPOLOGY "dyadic" \
        -N_NODES_PER_CPU 1
  done
done

# internode vs intranode
for REPLICATE in 0 1 2 3 4 5 6 7 8 9; do
  for ASYNCHRONOUS in 3; do
    export UITSL_EXEC_INSTANCE_UUID="$(cat /proc/sys/kernel/random/uuid)"
    mpiexec -n 2 \
      singularity exec docker://mmore500/dishtiny:sha-2f60ec6 \
      ./channel_selection \
        -RUN_SECONDS 330 \
        -REPLICATE "${REPLICATE}" \
        -N_THREADS 1 \
        -ASYNCHRONOUS "${ASYNCHRONOUS}" \
        -WHICH_TOPOLOGY "dyadic" \
        -N_NODES_PER_CPU 1
  done
done

# communication vs computation
for REPLICATE in 0 1 2 3 4 5 6 7 8 9; do
  for ASYNCHRONOUS in 3; do
    # ~0msec -> ~500msec
    for AMT_COMPUTE_WORK in 0 64 4096 262144 16777216; do
      export UITSL_EXEC_INSTANCE_UUID="$(cat /proc/sys/kernel/random/uuid)"
      mpiexec -n 2 \
        singularity exec docker://mmore500/dishtiny:sha-2f60ec6 \
        ./channel_selection \
          -RUN_SECONDS 330 \
          -REPLICATE "${REPLICATE}" \
          -N_THREADS 1 \
          -ASYNCHRONOUS "${ASYNCHRONOUS}" \
          -WHICH_TOPOLOGY "dyadic" \
          -N_NODES_PER_CPU 1 \
          -AMT_COMPUTE_WORK "${AMT_COMPUTE_WORK}"
    done
  done
done
