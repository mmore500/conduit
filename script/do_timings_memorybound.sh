#!/bin/bash

export PP_USE_OMP=0

export PP_NUM_UPDATES=8192
for SYNCHRONOUS in 0 1; do
  export PP_SYNCHRONOUS=$SYNCHRONOUS
  OUT_FILE="synchronous=${SYNCHRONOUS}+ext=.csv"
  echo "Threads,Work,Load,Replicate,Time" > $OUT_FILE
  for REP in {0..9}; do
    for LOAD_PER in 1 2 4 8 16 32; do
      for NUM_THREADS in 1 2 4 8 16 32; do

        echo "NUM_THREADS: ${NUM_THREADS}"
        export PP_NUM_THREADS=$NUM_THREADS

        RESISTANCE=64
        echo "RESISTANCE: ${RESISTANCE}"
        export PP_RESISTANCE=$RESISTANCE

        AMT_WORK=$(( $NUM_THREADS * $LOAD_PER ))
        echo "AMT_WORK: ${AMT_WORK}"

        GRID_SIZE=$(( $AMT_WORK * 64))
        echo "GRID_SIZE: ${GRID_SIZE}"
        export PP_GRID_SIZE=$GRID_SIZE

        echo "LOAD_PER: ${LOAD_PER}"
        echo "REP: ${REP}"

        /usr/bin/time -f "%e" -o tmp \
          ./pipe-profile \
          > /dev/null 2>&1
        ELAPSED_TIME=$(cat tmp)
        echo "ELAPSED_TIME: ${ELAPSED_TIME}"
        echo "${NUM_THREADS},${AMT_WORK},${LOAD_PER},${REP},${ELAPSED_TIME}" \
          >> $OUT_FILE
      done;
    done;
  done;
done
