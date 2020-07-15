#!/bin/bash

export LD_PRELOAD=/mnt/home/mmore500/libmimalloc.so

export PP_USE_OMP=0
export PP_TACITURN=1
export PP_NUM_SECONDS=5

unset PP_NUM_UPDATES
for SYNCHRONOUS in 0 1; do
  export PP_SYNCHRONOUS=$SYNCHRONOUS
  OUT_FILE="title=memorybound+synchronous=${SYNCHRONOUS}+ext=.csv"
  echo "Threads,Work,Load,Replicate,Unit Productivity" > $OUT_FILE
  for REP in {0..9}; do
    for LOAD_PER in 1 16 256 4096 65536 1048576; do
      for NUM_THREADS in 1 2 4 8 16 32; do

        MPI_PROCS=$(( $NUM_THREADS < 8 ? 1 : $NUM_THREADS / 8 ))
        echo "MPI_PROCS: ${MPI_PROCS}"

        echo "NUM_THREADS: ${NUM_THREADS}"
        export PP_NUM_THREADS=$(( $NUM_THREADS / $MPI_PROCS ))
        echo "PP_NUM_THREADS: ${PP_NUM_THREADS}"

        RESISTANCE=1
        echo "RESISTANCE: ${RESISTANCE}"
        export PP_RESISTANCE=$RESISTANCE

        AMT_WORK=$(( $NUM_THREADS * $LOAD_PER ))
        echo "AMT_WORK: ${AMT_WORK}"

        GRID_SIZE=$(( $AMT_WORK * 1 ))
        echo "GRID_SIZE: ${GRID_SIZE}"
        export PP_GRID_SIZE=$(( $GRID_SIZE / $MPI_PROCS ))

        echo "LOAD_PER: ${LOAD_PER}"
        echo "REP: ${REP}"
        echo "SYNCHRONOUS: ${SYNCHRONOUS}"

        mpiexec -n $MPI_PROCS ./pipe-profile > tmp
        UNIT_PRODUCTIVITY=$(cat tmp)
        echo "UNIT_PRODUCTIVITY: ${UNIT_PRODUCTIVITY}"
        echo "${NUM_THREADS},${AMT_WORK},${LOAD_PER},${REP},${UNIT_PRODUCTIVITY}" \
          >> $OUT_FILE
        echo
        echo "========================="
        echo
      done;
    done;
  done;
done
